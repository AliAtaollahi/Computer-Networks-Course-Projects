#include "room.hpp"
#include "manual.hpp"
#include "error.hpp"
#include "Utils.hpp"

using namespace std;

Room::Room(int number_, int status_, int price_, int max_capacity_, int capacity_, vector<std::shared_ptr<Rent>> rents_)
    :
    number(number_),
    status(status_),
    price(price_),
    max_capacity(max_capacity_),
    capacity(capacity_),
    rents(rents_)
{
}

string Room::get_info(DateConfig* today, bool is_admin, int filter_empty_status)
{
    if(filter_empty_status == EmptyFilterStatus::ENABLE && status == RoomStatus::FULL)
        return "";
    ostringstream info;
    info << "number: " << number << endl
         << "status: " << RoomStatus::str_form[status] << endl
         << "price: " << price << endl
         << "max_capacity: " << max_capacity << endl
         << "capacity: " << capacity << endl;
    
    if(is_admin && capacity <= (max_capacity/2))
        info << get_users_info(today);

    info << "################################" << endl;
    return info.str();
}

string Room::get_users_info(DateConfig* today)
{
    ostringstream info;
    info << "users:" << endl;
    for(auto rent : rents)
        if(rent->is_in_reservation(today))
            info << "   id: " << rent->person->get_id() << endl
                 << "   num_of_beds: " << rent->num_of_beds << endl
                 << "   reserveDate: " << rent->get_reserveDate() << endl
                 << "   checkoutDate: " << rent->get_checkoutDate() << endl;
    return info.str();
}

bool Room::has_room_num(int num)
{
    return num == number;
}

void Room::change_info(string new_max_capacity, string new_price)
{
    if(new_max_capacity != "" && stoi(new_max_capacity) < (max_capacity - capacity))
        throw Error(109);

    capacity += (new_max_capacity == "") ? capacity : (stoi(new_max_capacity) - max_capacity);
    max_capacity = (new_max_capacity == "") ?  max_capacity : stoi(new_max_capacity);
    price = (new_price == "") ?  price : stoi(new_price);
}

bool Room::has_user(DateConfig* today)
{
    for(auto rent : rents)
        if(Utils::calc_distance(today) <= Utils::calc_distance(rent->checkoutDate))
            return true;
    return false;
}

void Room::update_room(DateConfig* today)
{
    int occupied_beds = calc_occupied_beds(today);
    capacity = max_capacity - occupied_beds;
    status = (capacity == 0) ? RoomStatus::FULL : RoomStatus::EMPTY;
}

int Room::calc_occupied_beds(DateConfig* today)
{
    int occupied_beds = 0;
    for(auto rent : rents)
        if(rent->is_in_reservation(today))
            occupied_beds += rent->num_of_beds;
    return occupied_beds;
}

void Room::add_new_rent(int NumOfBeds, string CheckInDate, string CheckOutDate, Person* person, DateConfig* today)
{
    DateConfig CheckInDate_ = Utils::convert_to_DataConfig(CheckInDate);
    DateConfig CheckOutDate_ = Utils::convert_to_DataConfig(CheckOutDate);
    check_book_options(NumOfBeds, CheckInDate_, today);
    person->checkout(NumOfBeds * price);
    rents.push_back(make_shared<Rent>(person, NumOfBeds, CheckInDate_, CheckOutDate_));
}

void Room::check_book_options(int NumOfBeds, DateConfig CheckInDate, DateConfig* today)
{
    if(Utils::calc_distance(today) > Utils::calc_distance(CheckInDate))
        throw Error(503);
    if(NumOfBeds > max_capacity)
        throw Error(503);
    if(NumOfBeds > (max_capacity - calc_occupied_beds(&CheckInDate)))
        throw Error(109);
}

string Room::check_reserved_and_get_info(int id)
{
    ostringstream info;
    for(auto rent : rents)
        if(rent->person->has_this_id(id))
            info << "room_number=" << number << ' ' 
                 << "num_of_beds=" << rent->num_of_beds << ' ' 
                 << "reserve_date=" << rent->get_reserveDate() << ' '
                 << "checkout_date=" << rent->get_checkoutDate() << endl;
    return info.str();
}

void Room::cancel_book(int id, int number_of_canceling_bed, DateConfig* today)
{
    int rent_index = find_rent_index(id);
    shared_ptr<Rent> rent = rents[rent_index];
    check_cancel_options(number_of_canceling_bed, rent, today);
    if(rent->num_of_beds == number_of_canceling_bed)
        rents.erase(rents.begin() + rent_index);
    else
        rent->num_of_beds -= number_of_canceling_bed;
    
    rent->person->earn(number_of_canceling_bed * price / 2);
    update_room(today);
}

int Room::find_rent_index(int id)
{
    for(unsigned int i = 0; i < rents.size(); i++)
        if(rents[i]->person->has_this_id(id))
            return i;
    throw Error(102);
}

void Room::check_cancel_options(int number_of_canceling_bed, shared_ptr<Rent> rent, DateConfig* today)
{
    if(number_of_canceling_bed > rent->num_of_beds)
        throw Error(102);
    if(Utils::calc_distance(today) > Utils::calc_distance(rent->reserveDate))
        throw (Utils::calc_distance(today) <= Utils::calc_distance(rent->checkoutDate)) ? 
                Error(103) : Error(102);
}

void Room::leave_room(int id, DateConfig* today)
{
    int rent_index = find_rent_index(id);
    shared_ptr<Rent> rent = rents[rent_index];
    if(!rent->is_in_reservation(today))
        throw Error(112);
    rent->end_rent(today);
    update_room(today);
}

void Room::eject(int new_capacity, DateConfig* today)
{
    if(new_capacity > max_capacity)
        throw Error(503);
    if((max_capacity - capacity) > new_capacity)
    {
        int num_of_reject = (max_capacity - capacity) - new_capacity;
        for(auto rent : rents)
        {
            if(num_of_reject <= 0) 
                break;
            num_of_reject -= rent->num_of_beds;
            rent->end_rent(today);
        }
    }
    max_capacity = new_capacity;
}

FileDataContainers::RoomInfo Room::get_data_for_write()
{
    FileDataContainers::RoomInfo room_info;
    vector<FileDataContainers::RentInfo> users;
    for(auto rent : rents)
        users.push_back({rent->person->get_id(), rent->num_of_beds, 
                         Utils::convert_DataConfig_to_string(rent->reserveDate), 
                         Utils::convert_DataConfig_to_string(rent->checkoutDate)});
    room_info = {number, status, price, max_capacity, capacity, users};
    return room_info;
}

Rent::Rent(Person* person_, int num_of_beds_, DateConfig reserveDate_, DateConfig checkoutDate_)
    :
    person(person_),
    num_of_beds(num_of_beds_),
    reserveDate(reserveDate_),
    checkoutDate(checkoutDate_)
{
}

string Rent::get_reserveDate()
{
    ostringstream info;
    info << reserveDate.day << '-'
         << reserveDate.month << '-'
         << reserveDate.year;
    return info.str();
}

string Rent::get_checkoutDate()
{
    ostringstream info;
    info << checkoutDate.day << '-'
         << checkoutDate.month << '-'
         << checkoutDate.year;
    return info.str();
}

bool Rent::is_in_reservation(DateConfig* today)
{
    long int check_in_distance = Utils::calc_distance(reserveDate), 
             check_out_distance = Utils::calc_distance(checkoutDate), 
             check_today = Utils::calc_distance(today);
    
    return (check_today >= check_in_distance && check_today <= check_out_distance);
}

void Rent::end_rent(DateConfig* date)
{
    checkoutDate.day = date->day;
    checkoutDate.month = date->month;
    checkoutDate.year = date->year;
}


DateConfig::DateConfig(int day_, int month_, int year_)
    :
    day(day_),
    month(month_),
    year(year_)
{
}

void DateConfig::pass_day(string value)
{
    for (int i = 0; i < stoi(value); i++)
    {
        day++;
        if(day == 31)
        {
            day = 1;
            month++;
        }
        if(month == 13)
        {
            month = 1;
            year++;
        }
    }
}
#include <bits/stdc++.h>
#include <chrono>
#include "AS.hpp"
#include "Link.hpp"
#include "AsRelation.hpp"
#include "manual.hpp"

using namespace std;

int main()
{
    AS* as1 = new AS(1, {"111.0.0.0/8"});
    AS* as2 = new AS(2, {"112.0.0.0/8"});
    AS* as3 = new AS(3, {"113.0.0.0/8"});
    AS* as4 = new AS(4, {"114.0.0.0/8"});
    AS* as5 = new AS(5, {"115.0.0.0/8"});
    AS* as6 = new AS(6, {"116.0.0.0/8"});

    Link* link1 = new Link(as1, as2);
    Link* link2 = new Link(as1, as3);
    Link* link3 = new Link(as2, as3);
    Link* link4 = new Link(as2, as4);
    Link* link5 = new Link(as2, as5);
    Link* link6 = new Link(as3, as6);
    Link* link7 = new Link(as2, as6);

    AsRelation* AsRelation1_2 = new AsRelation(1, 2, link1, roles::PROVIDER);
    AsRelation* AsRelation2_1 = new AsRelation(2, 1, link1, roles::COSTUMER);
    
    AsRelation* AsRelation1_3 = new AsRelation(1, 3, link2, roles::PROVIDER);
    AsRelation* AsRelation3_1 = new AsRelation(3, 1, link2, roles::COSTUMER);

    AsRelation* AsRelation2_3 = new AsRelation(2, 3, link3, roles::PEER);
    AsRelation* AsRelation3_2 = new AsRelation(3, 2, link3, roles::PEER);

    AsRelation* AsRelation2_4 = new AsRelation(2, 4, link4, roles::PROVIDER);
    AsRelation* AsRelation4_2 = new AsRelation(4, 2, link4, roles::COSTUMER);

    AsRelation* AsRelation2_5 = new AsRelation(2, 5, link5, roles::PROVIDER);
    AsRelation* AsRelation5_2 = new AsRelation(5, 2, link5, roles::COSTUMER);

    AsRelation* AsRelation3_6 = new AsRelation(3, 6, link6, roles::PROVIDER);
    AsRelation* AsRelation6_3 = new AsRelation(6, 3, link6, roles::COSTUMER);

    AsRelation* AsRelation2_6 = new AsRelation(2, 6, link7, roles::PROVIDER);
    AsRelation* AsRelation6_2 = new AsRelation(6, 2, link7, roles::COSTUMER);

    as1->add_link(AsRelation1_2);
    as1->add_link(AsRelation1_3);

    as2->add_link(AsRelation2_1);
    as2->add_link(AsRelation2_3);
    as2->add_link(AsRelation2_4);
    as2->add_link(AsRelation2_5);
    as2->add_link(AsRelation2_6);

    as3->add_link(AsRelation3_1);
    as3->add_link(AsRelation3_2);
    as3->add_link(AsRelation3_6);

    as4->add_link(AsRelation4_2);

    as5->add_link(AsRelation5_2);

    as6->add_link(AsRelation6_3);
    as6->add_link(AsRelation6_2);

    as1->get_new_command("ads_all");
    as2->get_new_command("ads_all");
    as3->get_new_command("ads_all");
    as4->get_new_command("ads_all");
    as5->get_new_command("ads_all");
    as6->get_new_command("ads_all");

    as1->print_subnets();
    as2->print_subnets();
    as3->print_subnets();
    as4->print_subnets();
    as5->print_subnets();
    as6->print_subnets();



}
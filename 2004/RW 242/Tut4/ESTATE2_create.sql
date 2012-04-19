/* Creating tables for database ESTATE */

create table PERSON 
   (person_id         char(3)       not null,
    person_name       varchar(15)   not null,
    person_telno      char(10),
    primary key (person_id));
    
create table AGENT
   (agent_id           char(3)       not null,
    agent_regno        char(3)       not null,
    primary key (agent_id),
    foreign key (agent_id) references PERSON (person_id));
    
create table SELLER 
   (seller_id          char(3)       not null,
    primary key (seller_id),
    foreign key (seller_id) references PERSON (person_id));
	       
create table CLIENT 
   (client_id          char(3)       not null,
    client_maxprice    numeric       not null,
    primary key (client_id),
    foreign key (client_id) references PERSON (person_id));

create table PROPERTY 
   (prop_streetaddress  varchar(25)  not null,          
    prop_city           varchar(15)  not null,
    prop_no_of_rooms    integer,
    prop_price          numeric      not null,
    prop_owner          char(3)      not null,
    prop_agent          char(3),
    primary key (prop_streetaddress),
    foreign key (prop_owner) references SELLER(seller_id),
    foreign key (prop_agent) references AGENT(agent_id));
    
create table SHOW_DETAIL
   (show_num             integer     not null,
    show_client          char(3)      not null,
    show_property        varchar(25)  not null,
    show_date            date         not null,
    show_comments        varchar(25),
    primary key (show_num),
    foreign key (show_client) references CLIENT (client_id),
    foreign key (show_property) references PROPERTY (prop_streetaddress));

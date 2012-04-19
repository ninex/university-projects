create table tour
   (tour_number char(7) not null,
    tour_name varchar(50) not null,
    tour_operator varchar(25) not null,
    tour_lenght smallint not null,
    tour_capacity smallint not null,
    tour_desc varchar(1200) not null,
    primary key (tour_number));

create table attraction
   (attr_number char(7) not null,
    attr_name varchar(50) not null,
    attr_location varchar(15) not null,
    attr_desc char(1200) not null,
    attr_photo char(15) not null,
    primary key (attr_number));

create table has_attraction
   (tour_number char(7) not null,
    attr_number char(7) not null,
    onday smallint not null,
    primary key (tour_number,attr_number,onday),
    foreign key (tour_number) references tour,
    foreign key (attr_number) references attraction);

create table itinerary
   (tour_number char(7) not null,
    onday smallint not null,
    itin_overnight varchar(50),
    itin_desc varchar(1200),
    primary key (tour_number,onday),
    foreign key (tour_number) references tour);

create table tourrun
   (tourrun_number char(7) not null,
    tour_number char(7) not null,
    tourrun_begin date not null,
    tourrun_end date not null,
    tourrun_price numeric not null,
    primary key (tourrun_number),
    foreign key (tour_number) references tour);

create table client
    (client_num char(7) not null,
     client_lastname varchar(30) not null,
     client_firstname varchar(30) not null,
     client_address varchar(50) not null,
     client_phone char(10) not null,
     client_email varchar(50),
     client_cell char(10),
     client_password varchar(10),
     primary key (client_num));

create table booking
    (tourrun_number char(7) not null,
     client_num char(7) not null,
     booking_size smallint not null,
     booking_date date not null,
     primary key (tourrun_number,client_num),
     foreign key (tourrun_number) references tourrun,
     foreign key (client_num) references client);

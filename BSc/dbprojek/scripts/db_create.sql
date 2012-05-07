create table Gasteplaas (
  gp_ID integer not null,
  gp_Naam varchar(20) not null,
  gp_Provinsie varchar(20) not null,
  primary key (gp_ID));

create table Dorp (
  gp_ID integer not null,
  d_Naam varchar(20) not null,
  d_afstand integer not null,
  foreign key (gp_ID) references Gasteplaas);

create table Tipe_Aktiwiteite (
  akw_ID integer not null,
  akw_tipe varchar(20) not null,
  primary key (akw_ID));

create table Aktiwiteite (
  gp_ID integer not null,
  akw_ID integer not null,
  foreign key (gp_ID) references Gasteplaas,
  foreign key (akw_ID) references Tipe_Aktiwiteite);

create table Fasiliteite (
  f_ID integer not null,
  f_tipe varchar(20) not null,
  primary key (f_ID));

create table Verblyf_Fasiliteite (
  gp_ID integer not null,
  f_ID integer not null,
  vf_totaal integer not null,
  foreign key (gp_ID) references Gasteplaas,
  foreign key (f_ID) references Fasiliteite);

create table Kontak (
  k_ID char(13) not null,
  k_Naam varchar(30) not null,
  k_Adres varchar(50) not null,
  k_PosAdres varchar(50),
  k_ePos varchar(20),
  k_Tel varchar(18) not null,
	k_Tel2 varchar(18),
  primary key (k_ID));

create table Bespreking (
  b_ID char(5) not null,
  k_ID char(13) not null,
  b_Begin date not null,
  b_Eindig date not null,
  gp_ID integer not null,
  primary key (b_ID),
  foreign key (k_ID) references Kontak,
  foreign key (gp_ID) references Gasteplaas);

create table Slaap_plek (
  b_ID  char(5) not null,
  f_ID integer not null,
  s_aantal integer not null,
  foreign key (b_ID) references Bespreking,
  foreign key (f_ID) references Fasiliteite);

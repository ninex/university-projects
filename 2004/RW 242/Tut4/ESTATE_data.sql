/* Inserting data into database ESTATE */

insert into PERSON
  values('P01', 'Black', '111 111111');
insert into PERSON
  values('P02', 'Brown', '222 222222');
insert into PERSON
  values('P03', 'Blue', '333 333333');
insert into PERSON
  values('P04', 'Red', null);
insert into PERSON
  values('P05', 'Orange', null); 
insert into PERSON
  values('P06', 'White', '666 666666');
insert into PERSON
  values('P07', 'Grey', '777 777777');
	    
insert into CLIENT
  values('P05', 500000);
insert into CLIENT
  values('P02', 700000);
insert into CLIENT
  values('P06', 600000);

insert into SELLER
  values('P01');
insert into SELLER
  values('P02');
insert into SELLER
  values('P03');

insert into AGENT
  values('P03', 'A01');
insert into AGENT
  values('P07', 'A02');

insert into PROPERTY
  values('120 Surf Drive', 'Honolulu', 3, 500000, 'P01', 'P07');
insert into PROPERTY
  values('360 Sail Street', 'Orlando', 4, 650000, 'P01', 'P07');
insert into PROPERTY
  values('400 Hill Road', 'Aspen', 3, 470000, 'P02', 'P03');
insert into PROPERTY
  values('100 Mountain Way', 'Jackson', 3, 333000, 'P03', null);

insert into SHOW_DETAIL
  values(1, 'P02', '120 Surf Drive', '08/26/2004', null);
insert into SHOW_DETAIL
  values(2, 'P02', '360 Sail Street', '08/26/2004', null);
insert into SHOW_DETAIL
  values(3, 'P05', '120 Surf Drive', '08/26/2004', null);
insert into SHOW_DETAIL
  values(4, 'P05', '100 Mountain Way', '08/26/2004', null);

  

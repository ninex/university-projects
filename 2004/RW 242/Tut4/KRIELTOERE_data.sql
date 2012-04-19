insert into tour
	values('TOUR01','Tour Tuscany','Vacation Italy',5,100,'Spend 5 unforgettable days exploring this specific region of Italy.  Picture rolling hills covered in vineyards, medieval towns perched high on hilltops and unbelievable sunsets on the horizon.  That is exactly what you will get on this tour.  Visit the must see cities Rome and Florence as well as spend time going to some out of the way places.');
insert into tour
	values('TOUR02','London Theatre Tour','Theatre Breaks',3,30,'See the sights of London on this 3 day tour which includes tickets to 2 West End performances');
insert into tour
	values('TOUR03','London and Paris','Evans',4,50,'Spend 2 nights each in 2 of Europe''s most beautiful cities');


insert into attraction
	values('ATTR01','Phantom of the Opera','London','Performance Length: 2h30m 
	Theatre: Her Majesty''s, Haymarket, London 
	Andrew Lloyd Webber''s romantic musical is based on Gaton Leroux''s gothic novel of life beneath the stage of the Paris Opera where a mysterious man falls in love with Christine, a singer who inspires ''the music of the night''','attr01.jpg');
insert into attraction
	values('ATTR02','The Lion King','London','Performance Length: 2h45m 
	Theatre: Lyceum Theatre, Wellington St, London 
	Simba, Prince of Lions, enjoys his life as the heir apparent of the pridelands.  When his evil uncle Scar kills his father and takes the queen as his wife things change.  Scar tries to encourage Simba to his death, by making a pact with some hyenas.  But the young lion manages to escape and is rescued by Timon and Pumba, a meercat and warthog who teach Simba their No Worries philosophy.  When he is fully grown he returns to fight Scar for his kingdom.
	Julie Taymor directs the stage adaptation of Walt Disney''s film of the same name.  Songs by Tim Rice and Elton John ','attr02.jpg');
insert into attraction
	values('ATTR03','London Bus Tour','London','See the main sights of London on a 3 hour bus tour','attr03.jpg');
insert into attraction
	values('ATTR04','Tate Modern','London','Tate Modern is Britain''s new national museum of modern art.
	Housed in the former Bankside Power Station, Tate Modern displays the Tate collection of international modern art from 1900 to the present day, including major works by Dali, Picasso, Matisse, Rothko and Warhol as well as comtemporary work by artists such as Dorothy Cross, Gilbert & George and Susan Hiller','attr04.jpg');
insert into attraction
	values('ATTR05','London Eye','London','For a bird''s-eye view of the city, hop aboard the London Eye, the 450-feet Ferris wheel built on the banks of the Thames as part of the millennium celebation.  The ride is 30 minutes long with spectacular views of Big Ben and the houses of Parliament just across the river, St. Paul''s Cathedral to the east and the river traffic just below','attr05.jpg');
insert into attraction
	values('ATTR06','Spanish Steps','Rome','The Piazza di Spagna takes its name from the Spanish Embassy which was established there in the seventeenth century.  Beyond the Barcaccia Fountain rise the beautiful Scalinata della Trinita dei Monti, or the Spanish Steps.  The staircase contains 138 steps of travertine stone rising in three successive flights.  It was finished by 1726 and became a favorite rendezvous for artists and their models, most of them peasants from the Campagna south of Rome.  Today it is still one of the most popular meeting-places in Rome for foreigners and Italians alike','attr06.jpg');
insert into attraction
	values('ATTR07','Trevi Fountain','Rome','Legend says that if you toss a coin into the fountain over your shoulder, you will one day return to the eternal city','attr07.jpg');
insert into attraction
	values('ATTR08','Castle Meleto','Siena','The powerful structure of the Castle of Meleto rises on a gentle hill, in the heart of the Chianti Classico area.
	Ever since its construction, it has always belonged to the monks of the Abbey of Coltibuono and has marked the history of one of the most important families of the Chianti area, the Ricasoli, who have embellished and modified its structure over the centuries.
	Today the castle still keeps the mark of the massive fortifications of the fifteenth century, the imposing tower built with a cylindrical structure to dodge the shots of the artillery, but also the distinguished grace of the eighteenth century villa with its succession of rooms decorated and furnished as dictated by the typical taste of the country aristocracy of Tuscany','attr08.jpg');
insert into attraction
	values('ATTR09','Tour of Montepulciano','Siena','As well as for its architecture, Montepulciano is also famous for itDs prestigious wine, the Vino Nobile.  After a little tour of the town, you will visit a winery and enjoy a wine tasting','attr09.jpg');
insert into attraction
	values('ATTR10','Eiffel Tower','Paris','The Eiffel Tower was built for the International Exhibition of Paris of 1889 commemorating the centenary of the French Revolution. Of the 700 proposals submitted in a design competition, Gustave Eiffel''s was unanimously chosen.  However, it was not accepted by all at first, and a petition of 300 names - including those of Maupassant, Emile Zola, Charles Garnier, and Dumas the Younger - protested its construction.
	At 300 metres (320.75m including antenna), and 7000 tons, it was the world''s tallest building until 1930','attr10.jpg');
insert into attraction
	values('ATTR11','Arc de Triomphe','Paris','Commissioned in 1806 by Napoleon, shortly after his victory at Austerlitz, it was not finished until 1836.  There are four huge relief sculptures at the bases of the four pillars.  These commemorate The Triumph of 1810 (Cortot); Resistance, and Peace (both by Etex); and The Departure of the volunteers, more commonly known by the name La Marseillaise (Rude).
	Engraved around the top of the Arch are the names of major victories won during the Revolutionary and Napoleonic periods.  The names of less important victories, as well as those of 558 generals, are to be found on the inside walls.  Generals whose names are underlined died in action.
	Beneath the Arch is the Tomb of the Unknown Soldier, and eternal flame commemorating the dead of the two world wars.
	','attr11.jpg');
insert into attraction
	values('ATTR12','Notre Dame Cathedral','Paris','Notre-Dame dates back to the 13th century and is one of the masterpieces of Gothic art in Western Europe.  Its stainglasses and the huge interior are really stunning artistic experiences of mystical dimension.  Located on the Cite island and surrounded by the Seine river, Notre-Dame is a flagship in the Parisian landscape and provides a magnificent view of the city from the top of its towers.','attr12.jpg');
insert into attraction
	values('ATTR13','Louvre','Paris','Established in 1793 by the French Republic, the Louvre Museum is one of the earliest Europen museums.
	Divided into 7 departments, the Louvre collections incorporate works dating from the birth of the great antique civilisations right up to the first half of the XIXth century, thereby confirming its encyclopedic vocation.','attr13.jpg');

insert into has_attraction values('TOUR01','ATTR06',1); insert into has_attraction values('TOUR01','ATTR07',2); insert into has_attraction values('TOUR01','ATTR09',3); insert into has_attraction values('TOUR01','ATTR08',4); insert into has_attraction values('TOUR02','ATTR03',1);
insert into has_attraction values('TOUR02','ATTR01',2); insert into has_attraction values('TOUR02','ATTR04',2); insert into has_attraction values('TOUR02','ATTR02',3); insert into has_attraction values('TOUR03','ATTR03',2); insert into has_attraction values('TOUR03','ATTR05',2);
insert into has_attraction values('TOUR03','ATTR10',3); insert into has_attraction values('TOUR03','ATTR11',4); insert into has_attraction values('TOUR03','ATTR12',4); insert into has_attraction values('TOUR03','ATTR13',4);

insert into itinerary
	values('TOUR01',1,'Hotel Jonella','Welcome to Rome.  Make your own way to the hotel, the day is then yours to discover Rome at leisure.  Tonight, enjoy a welcome drink at the hotel as you meet your tour guide and fellow travellers.  Stopover at Hotel Jonella.');
insert into itinerary
	values('TOUR01',2,'Hotel Jonella','Walking tour of Rome seeing the Colosseum, Roman Forum, Trevi fountain, the Pantheon and Piazza Navona.  Dinner at a local restaurant in the ancient heart of the city. Stopover at Hotel Jonella.');
insert into itinerary
	values('TOUR01',3,'Le Casacce','After breakfast travel to Siena stopping for a guided tour of the elegant town of Montepulciano.  Spend the evening relaxing at LeCasacce. Stopover at Le Casacce.
	Attractions: Tour of Montepulciano + Wine Tasting');
insert into itinerary
	values('TOUR01',4,'Le Casacce','Today we experience the essential Tuscany.  We spend the morning exploring the region by bike.  Then we take a trip to Castello Meleto for wine tasting and lunch.  Our day of activity ends with a dip in the pool to cool off and a sunset dinner at Buonconvento. Stopover at Le Casacce.');
insert into itinerary
	values('TOUR01',5,null,'Travel to Florence.  The afternoon is yours to explore this fabulous city, maybe visit the Academia Gallery and see Michaelangelo''s David or perhaps stroll through the famous Uffizi Gallery.');

insert into itinerary
	values('TOUR02',1,'Strand Palace','You will be collected from Heathrow airport by a Theatre Breaks representative and taken to the Strand Palace Hotel.  After lunch at the hotel, the tour starts with a bus tour of London and a ride on the London Eye. Stopover at Strand Palace.');
insert into itinerary
	values('TOUR02',2,'Strand Palace','This morning we visit the Tate Modern gallery, the afternoon is yours to explore London.  How about catching a taxi to Oxford street for some shopping.  Tonight we see probably London''s most famous theatre production, Phantom of the Opera. Stopover at Strand Palace.');
insert into itinerary
	values('TOUR02',3,'Strand Palace','This afternoon enjoy watching The Lion King, currently the number 1 musical in London. Stopover at Strand Palace.');

insert into itinerary
	values('TOUR03',1,'Queens Park Hotel','Check into the hotel after 2pm and the remainder of the day is free to relax before heading to Piccadilly for a short walk to explore nearby Covent Garden. Stopover at Queen''s Park Hotel.');
insert into itinerary
	values('TOUR03',2,'Queens Park Hotel','Explore London by bus and enjoy a ride on the London Eye.  After dinner see a musical in the West End. Stopover at Queen''s Park Hotel.');
insert into itinerary
	values('TOUR03',3,'Hotel de Varenne','Leaving London, we head for Dover and our ferry to France.  Enjoy a first view of the city''s sights on our evening illuminations tour, stopping at the Eiffel Tower. Stopover at Hotel de Varenne.');
insert into itinerary
	values('TOUR03',4,'Hotel de Varenne','Paris sightseeing.  We begin the day with our Paris sight-seeing tour including Notre-Dame, the Arc de Triomphe and the Louvre.  After lunch, we take a short drive to Versailles where you will take a guided tour of the palace. Stopover at Hotel de Varenne.');

insert into tourrun values('RUN001','TOUR01','06/21/2002','06/25/2002',650); insert into tourrun values('RUN002','TOUR01','07/12/2002','07/16/2002',650);
insert into tourrun values('RUN003','TOUR01','08/02/2002','08/06/2002',650); insert into tourrun values('RUN004','TOUR01','08/23/2002','08/27/2002',650);
insert into tourrun values('RUN005','TOUR01','09/13/2002','09/17/2002',540); insert into tourrun values('RUN006','TOUR01','10/04/2002','10/08/2002',540);
insert into tourrun values('RUN007','TOUR02','06/21/2002','06/23/2002',550); insert into tourrun values('RUN008','TOUR02','07/12/2002','07/14/2002',550);
insert into tourrun values('RUN009','TOUR02','08/02/2002','08/04/2002',550); insert into tourrun values('RUN010','TOUR02','08/23/2002','08/25/2002',550);
insert into tourrun values('RUN011','TOUR02','09/13/2002','09/15/2002',475); insert into tourrun values('RUN012','TOUR02','10/04/2002','10/06/2002',475);
insert into tourrun values('RUN013','TOUR03','06/21/2002','06/24/2002',580); insert into tourrun values('RUN014','TOUR03','07/12/2002','07/15/2002',580);
insert into tourrun values('RUN015','TOUR03','08/02/2002','08/05/2002',580); insert into tourrun values('RUN016','TOUR03','09/13/2002','09/16/2002',510);

insert into client
	values('CLN0001','Cantor','Georg', '28 CompanjeLaan, Somerset-Wes', '0218552902','georg@stikland.org','1234567890','aleph');
insert into client
	values('CLN0002','Dubois','Daniel', '28 CompanjeLaan, Somerset-Wes', '0218552902','georg@stikland.org','1234567890','aleph');
insert into client
	values('CLN0003','Jackson','Jeremy', '28 CompanjeLaan, Somerset-Wes', '0218552902','georg@stikland.org','1234567890','aleph');
	
insert into booking
	values('RUN003','CLN0001',20, '04/04/2002');
insert into booking
	values('RUN001','CLN0001',10, '04/04/2002');
insert into booking
	values('RUN004','CLN0002',5, '04/04/2002');
insert into booking
	values('RUN001','CLN0002',5, '04/04/2002');


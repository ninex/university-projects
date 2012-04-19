public class Dating{

	public static void main (String[] args){
		int count = 0;
		int count2 = 0;
		Man men[] = new Man[10];
		while (count < men.length)
		{
			men[count] = new Man();
			count++;
		}
		Human.createHumans(men);
		Woman w = new Woman();
		w.setName("Eve");
		w.setAge(25);

		//Create new man objects using a loop

		//Initialise the men by calling createHumans();

		while (count2 < 3){
			count = 0;
			// Let the woman live for a day: Human.liveADay(w);
			Human.liveADay(w);
			// Use a loop to let the men live for a day
			while (count < men.length)
			{
				Human.liveADay(men[count]);
				count++;
			}
			// Get a dating partner for the woman by calling getDatingPartner()
			w.partner = w.getDatingPartner(men);
			// Print the detail of the woman.
			w.printDetail(true);
			System.out.println();
			count2++;
		}
	}
}

//Abstract class Human
public abstract class Human{
	private String name = "";
	private int age = 0;
	private int money = 0;
	private int health = 0;
	private int personality = 0;
	private int selfcontrol = 0;
	private int intelligence = 0;
	Human partner = null;

	/**
	* Sets the name and age fields of the humans to random vallues
	*/
	public static void createHumans(Human[] h){
		int count = 0;

	// Use a while-loop and the random function in the Math class to
	// generate random names and ages for the list of humans h.
	// For example, names can be created as h[count].setName("Adam" + count);
	// where count is used to count the number of persons in the list.
	if (h.length != 0)
	{
		while (count < h.length)
		{
			h[count].name = "NineX" + count;
			h[count].age = (int)((Math.random() *100));
			count++;
		}
	}
	}

	/**
	*	This method must be implemented in a specialised manner for each subclass.
	Directory*/
	public abstract int evaluate();


	/**
	* Calculates which human has the highest evaluation value
	*/
	public Human getDatingPartner(Human[] h){

		// Use a loop to call each human in h's evaluate function and
		// return a reference to the human with the maximun evaluation value.
	int count = 0;
	int maxEval = 0;
	int i;
	int tempPartner = 0;
	if (h.length != 0)
	{

		while (count < h.length)
		{
			if ((i = h[count].evaluate()) > maxEval)
			{
				maxEval = i;
				tempPartner = count;
			}
			count++;
		}
		return h[tempPartner];
	}else
	{
		return null;
	}
	}

	/**
	* Performs all the activities that a human can do in a day.
	*/
	public static void liveADay(Human h){

		h.work();
		h.drink();
		h.exercise();
		h.specialise();
		h.sleep();
	}

	/**
	* This functionsetHealth(getHealth() + 5);
		setCoolness(getCoolness() + 9);
		setPersonality(getPersonality() + 6); calls extra methods defined in subclasses of human. These
	* methods are extra activities a subclass can perform during the day.
	*/
	public abstract void specialise();

	/**
	* Prints extra detail that is specific to a subclass
	*/
	public abstract void printSpecial();

	/**
	* Prints all the detail of a human
	*/
	public void printDetail(boolean printPartner){

		// Print human detail
		// print subclass detail (printSpecial();)
		// print partner detail (this.partner.printDetail();)
		System.out.println("Name :" + name);
		System.out.println("Age :" + age);
		System.out.println("Money :" + money);
		System.out.println("Health :" + health);
		System.out.println("Personality :" + personality);
		System.out.println("Selfcontrol :" + selfcontrol);
		System.out.println("Intelligence :" + intelligence);
		printSpecial();
		System.out.println();
		if ((partner != null) && (printPartner))
		{
			System.out.println("Partner detail:");
			partner.printDetail(false);
		}
	}


	public void setName(String name){

		this.name = name;
	}


	public String getName(){

		return name;
	}

	public void setAge(int a){

		if (a > 0){
			age = a;
		}
	}

	public int getAge(){ return age;}

	public abstract void work();

	public abstract void drink();

	public abstract void exercise();

	public abstract void sleep();

	/**
	* Sets money to a new value
	* All values must be between 0 and 100.
	*/
	public void setMoney(int m){
		if ((m>=0) & (m<=100))
		{
			money = m;
		}
	}

	/**
	* Sets health to a new value
	* All values must be between 0 and 100.
	*/
	public void setHealth(int h){
		if ((h>=0) & (h<=100))
		{
			health = h;
		}
	}

	/**
	* Sets personality to a new value
	* All values must be between 0 and 100.
	*/
	public void setPersonality(int p){
		if ((p>=0) & (p<=100))
		{
			personality = p;
		}
	}

	/**
	* Sets selfcontrol to a new value
	* All values must be between 0 and 100.
	*/
	public void setSelfcontrol(int s){
		if ((s>=0) & (s<=100))
		{
			selfcontrol = s;
		}
	}

	/**
	* Sets intelligence to a new value
	* All values must be between 0 and 100.
	*/
	public void setIntelligence(int i){
		if ((i>=0) & (i<=100))
		{
			intelligence = i;
		}

	}

	public int getMoney(){ return money; }
	public int getHealth(){ return health; }
	public int getPersonality(){ return personality; }
	public int getSelfcontrol(){ return selfcontrol; }
	public int getIntelligence(){ return intelligence; }
	
} 

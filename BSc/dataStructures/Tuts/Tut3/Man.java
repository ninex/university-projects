public class Man extends Human{
	private int coolness = 0;

	/**
  * Returns the value of a man defined by a function on the instance
  * varialbles age, money, health, personality, selfcontrol, intelligence and
  * beauty.
  */
	public int evaluate(){
	
		return getCoolness();
	}

	public void work()
	{
		setMoney(getMoney() + (int)((Math.random()*9)));
		setSelfcontrol(getSelfcontrol() + (int)((Math.random()*7)));
	}

	public void drink()
	{
		setHealth(getHealth() - (int)((Math.random()*5)));
		setCoolness(getCoolness() + (int)((Math.random()*5)));
		setMoney(getMoney() - (int)((Math.random()*5)));
		setIntelligence(getIntelligence() + (int)((Math.random()*12)));
	}

	public void exercise()
	{
		setHealth(getHealth() + (int)((Math.random()*10)));
		setCoolness(getCoolness() - (int)((Math.random()*8)));
	}

	public void sleep()
	{
		setHealth(getHealth() + (int)((Math.random()*5)));
		setCoolness(getCoolness() + (int)((Math.random()*4)));
	}

		/**
  * Updates the properties of a man influenced by surfing.
  */
	public void surf(){
		setHealth((int)((Math.random()*80)));
		setCoolness((int)((Math.random()*60)));
		setPersonality((int)((Math.random()*50)));
	}

	/**
  * Updates the properties of a man influenced by fist fighting.
  */
	public void fistFight(){
		setCoolness(getCoolness() - (int)((Math.random()*20)));
		setPersonality(getPersonality() + (int)((Math.random()*5)));
		setHealth((getHealth() - (int)(Math.random()*5)));

	}
	/**
  * Runs the activities of man during the day
  */
	public void specialise(){
		surf();
		fistFight();
	}

	/**
  * Prints the instance variables associated with a man
  */
	public void printSpecial(){
		System.out.println(coolness);
	}


	public int getCoolness(){

		return coolness;
	}

	/**
  * Set the coolness factor of a man
  * Value must be between 0 and 100
  */
	public void setCoolness(int c){
		if ((c>=0) & (c<=100))
		{
			coolness = c;
		}
	}




}

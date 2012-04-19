public class Woman extends Human{
	private int beauty = 0;

	/**
	* Returns the value of a woman defined by a function on the instance
	* varialbles age, money, health, personality, selfcontrol, intelligence and
	* beauty.
	*/ 				
	public int evaluate(){
					
		return getBeauty() + getPersonality() - getIntelligence() - getSelfcontrol();
	}

	/**
	*	Prints the instance variables associated with a woman
	*/
	public void printSpecial(){
		System.out.println("Beauty :" + beauty);
	}

	/**
	* Set the beauty value of a woman
	* Value must be between 0 and 100
	*/
	public void setBeauty(int b){
		if ((b>=0) & (b<=100))
		{
			beauty = b;
		}
	}

	public int getBeauty(){

		return beauty;
	}

	/**
	*	Runs the activities of woman during the day
	*/
	public void specialise(){

		aerobics();
		nag();
	}

	/**
	* Updates the properties of a woman influenced by doing work.
	*/
	public void work(){

		setMoney(getMoney() + (int)((Math.random()*15)));
		setSelfcontrol(getSelfcontrol() - 5);
	}

	/**
	* Updates the properties of a woman influenced by drinking.
	*/
	public void drink(){
		setHealth(getHealth() - (int)((Math.random()*10)));
		setMoney(getMoney() - (int)((Math.random()*10)));
		setIntelligence(getIntelligence() - (int)((Math.random()*12)));
		setBeauty(getBeauty() + (int)((Math.random()*20)));
	}

	/**
	* Updates the properties of a woman influenced by exercisig.
	*/
	public void exercise(){
		setHealth(getHealth() + (int)((Math.random()*10)));
		setBeauty(getBeauty() + (int)((Math.random()*15)));
	}

	/**
	* Updates the properties of a woman influenced by sleeping.
	*/
	public void sleep(){
		setBeauty(getBeauty() + (int)((Math.random()*5)));
	}

	/**
	* Updates the properties of a woman influenced by doing aerobics.
	*/
	public void aerobics(){
		setBeauty(getBeauty() + (int)((Math.random()*5)));
	}

	/**
	* Updates the properties of a woman influenced by naging.
	*/
	public void nag(){
		setBeauty(getBeauty() - (int)((Math.random()*15)));
		setIntelligence(getIntelligence() - (int)((Math.random()*2)));
		setSelfcontrol(getSelfcontrol() + (int)((Math.random()*15)));
		setPersonality(getPersonality() - (int)((Math.random()*5)));
	}
}

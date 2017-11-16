//this interface defines a function that takes two int
//arguments and returns an int result
//thus, it can describe any binary operation on
//two into that returns an int

package userfuncs.binaryfuncs;

public interface BinaryFunc{
	
	//obtain the name of the function
	public String getName();
	
	//this is the function to perform
	//it'll be provided by specific implementations
	public int func(int a, int b);
}

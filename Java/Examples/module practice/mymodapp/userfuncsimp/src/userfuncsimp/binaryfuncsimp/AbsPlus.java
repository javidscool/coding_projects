//AbsPlus provides a concrete implementation of BinaryFunc
//it returns the result of abs(a) + abs(b)

package userfuncsimp.binaryfuncsimp;

import userfuncs.binaryfuncs.BinaryFunc;

public class AbsPlus implements BinaryFunc{
	//return name of this function
	public String getName(){
		return "absPlus";
	}
	
	//implement the AbsPlus function
	public int func(int a, int b){
		return Math.abs(a) + Math.abs(b);
	}
}

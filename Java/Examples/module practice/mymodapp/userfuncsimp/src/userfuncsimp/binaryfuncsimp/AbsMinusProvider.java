//this is a provider for the AbsMinus function

package userfuncsimp.binaryfuncsimp;

import userfuncs.binaryfuncs.*;

public class AbsMinusProvider implements BinFuncProvider{
	
	//provide an AbsMinus object
	public BinaryFunc get(){
		return new AbsMinus();
	}
}

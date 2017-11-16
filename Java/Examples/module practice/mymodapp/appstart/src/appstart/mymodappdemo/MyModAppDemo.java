//a module-based application that demonstrates 
//services and service providers

package appstart.mymodappdemo;

import java.util.ServiceLoader;

import appfuncs.simplefuncs.SimpleMathFuncs;
import userfuncs.binaryfuncs.*;

public class MyModAppDemo{
	public static void main(String[] args){
		
		if(SimpleMathFuncs.isFactor(2, 10)) {
			System.out.println("2 is a factor of 10");
		}
		
		System.out.println("Smallest factor common to both 35 and 105 is " + SimpleMathFuncs.lcf(35, 105));
		
		System.out.println("Largest factor common to both 30 and 50 is " + SimpleMathFuncs.gcf(30, 50));
		
		//now, use service-based, user-defined operations
		
		//get a service loader for binary functions
		ServiceLoader<BinFuncProvider> ldr = ServiceLoader.load(BinFuncProvider.class);
		
		BinaryFunc binOp = null;
		
		//find the provider for absPlus and obtain the function
		for(BinFuncProvider bfp : ldr){
			if(bfp.get().getName().equals("absPlus")){
				binOp = bfp.get();
				break;
			}
		}
		
		if(binOp != null){
			System.out.println("Result of absPlus function: " + binOp.func(-9, -4));
		}
		else{
			System.out.println("absPlus function not found");
		}
		
		//now, find the provider for absMinus and obtain the function
		for(BinFuncProvider bfp : ldr){
			if(bfp.get().getName().equals("absMinus")){
				binOp = bfp.get();
				break;
			}
		}
		
		if(binOp != null){
			System.out.println("Result of absMinus function: " + binOp.func(-9, 4));
		}
		else{
			System.out.println("absMinus function not found");
		}
		
		Module myMod = MyModAppDemo.class.getModule();
		System.out.println("Module is " + myMod.getName());
		
		System.out.print("Packages: ");
		for(String pkg : myMod.getPackages()){
			System.out.println(pkg + " ");
		}
	}

}

/**
 * 
 */
/**
 * @author javidscool
 *
 */
module appstart{
	requires appfuncs;
	requires userfuncs;
	
	//appstart now uses BinFuncProvider
	uses userfuncs.binaryfuncs.BinFuncProvider;
}
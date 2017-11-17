import java.util.*;

//Demonstrate a resource bundle.
class LRBDemo {
	public static void main(String args[]) {

		//Load the default bundle.
		ResourceBundle rd = ResourceBundle.getBundle("SampleRB");
		
		System.out.println("English version: ");
		System.out.println("String for Title key : " + rd.getString("title"));
		System.out.println("String for StopText key: " + rd.getString("StopText"));
		System.out.println("String for StartText key: " + rd.getString("StartText"));
		
		//Load the German bundle.
		rd = ResourceBundle.getBundle("SampleRB", Locale.GERMAN);
		
		System.out.println("\nGerman version: "); 
		System.out.println("String for Title key : " + rd.getString("title"));
		System.out.println("String for StopText key: " + rd.getString("StopText"));
		System.out.println("String for StartText key: " + rd.getString("StartText"));
	}
}
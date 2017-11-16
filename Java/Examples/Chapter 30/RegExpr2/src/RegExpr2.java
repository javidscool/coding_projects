// Use find() to find a subsequence.
import java.util.regex.*;
class RegExpr2 {
	public static void main(String args[]) {
		Pattern pat = Pattern.compile("Java");
		Matcher mat = pat.matcher("Java 8");
		
		System.out.println("Looking for Java in Java 8.");
		
		if(mat.find()) System.out.println("subsequence found");
		else System.out.println("No Match");
		
		pat = Pattern.compile("test");
		mat = pat.matcher("test 1 2 3 test");
		while(mat.find()) {
			System.out.println("test found at index " +
					mat.start());
		}
	}
}
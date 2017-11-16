// Use wildcard and quantifier.
import java.util.regex.*;
class RegExpr3 {
	public static void main(String args[]) {
		Pattern pat = Pattern.compile("e.+?d");
		Matcher mat = pat.matcher("extend cup end table");
		while(mat.find())
			System.out.println("Match: " + mat.group());
		
		String str = "Jon Jonathan Frank Ken Todd";
		pat = Pattern.compile("Jon.*? ");
		mat = pat.matcher(str);
		
		System.out.println("Original sequence: " + str);
		str = mat.replaceAll("Eric ");
		System.out.println("Modified sequence: " + str);
		
		// Match lowercase words.
		pat = Pattern.compile("[ ,.!]");
		String strs[] = pat.split("one two,alpha9 12!done.");
		for(int i=0; i < strs.length; i++)
			System.out.println("Next token: " + strs[i]);
	}
}
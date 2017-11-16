import java.io.*;
import java.nio.file.*;
class ShowFile {
	public static void main(String args[]){

		int i;

		// Open the file and obtain a stream linked to it.
		try ( InputStream fin = Files.newInputStream(Paths.get("test.txt")) ){
			do {
				i = fin.read();
				if(i != -1) System.out.print((char) i);
			} while(i != -1);
		} catch(InvalidPathException e) {
			System.out.println("Path Error " + e);
		} catch(IOException e) {
			System.out.println("I/O Error " + e);
		}
	}
}

import java.util.Arrays;
import java.util.concurrent.SubmissionPublisher;  
import java.util.concurrent.locks.Condition;

public class MyMainApp implements Runnable{
		
	MyMainApp() {
		// thread created
		Thread t = new Thread(this, "Admin Thread");
		
		// this will call run() function
		t.start();
	}
	
	public void run() {
		//Create Publisher 
		SubmissionPublisher<String> publisher = new SubmissionPublisher<>();  
		
		MyTransformProcessor<String, Integer> transformProcessor = new MyTransformProcessor<>(s -> Integer.parseInt(s));
				 
		//Register Subscriber (it’s the class we wrote before)
		MySubscriber<Integer> subscriber = new MySubscriber<>();
		//chain processor to be in middle of publisher and subscriber 
		publisher.subscribe(transformProcessor);  
		transformProcessor.subscribe(subscriber);
		
		//Publish items  
		System.out.println("Publishing Items...");  
		String[] items = {"1", "2", "3", "4", "5", "6", "7", "8"};
		Arrays.asList(items).stream().forEach(i -> publisher.submit(i));
		publisher.close(); 
	}
	
	public static void main(String[] args){
		System.out.println("hello world");
		
		try {
			new MyMainApp();
			
			//currentThread() returns a reference to the currently executing thread
			//in this case that's our separate thread
			Thread.currentThread().join();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

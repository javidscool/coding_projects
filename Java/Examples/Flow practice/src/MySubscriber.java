import java.util.concurrent.Flow.*;  
  
public class MySubscriber<T> implements Subscriber<T>{
	private Subscription subscription;  
  
  	@Override  
  	public void onSubscribe(Subscription subscription){  
    	this.subscription = subscription;  
    
		subscription.request(1); 
  	}  
  
  	@Override  
  	public void onNext(T item) {  
    	System.out.println("Got : " + item);  
    		
		subscription.request(1); 
  	}  
  
  	@Override  
  	public void onError(Throwable t) {  
    	t.printStackTrace();  
  	}  
  
  	@Override  
  	public void onComplete() {  
    	System.out.println("Done");  
 	 }  
}

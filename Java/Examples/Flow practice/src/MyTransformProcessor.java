import java.util.concurrent.Flow.*;  
import java.util.concurrent.SubmissionPublisher;
import java.util.function.Function;  

public class MyTransformProcessor<T,R> extends SubmissionPublisher<R> implements Processor<T, R> {  
  
	//function takes in type T and returns type R
	private Function<? super T, ? extends R> function;  
	private Subscription subscription;  
  
	public MyTransformProcessor(Function<? super T, ? extends R> function) {  
		super();  
		this.function = function;  
	}  
  
	@Override  
	public void onSubscribe(Subscription subscription) {  
		this.subscription = subscription;  
		subscription.request(1);  
	}  
  
	@Override  
	public void onNext(T item) {  
		//put the item in the stream with the submit() function after 
		//having processed it in the transform function we took in
		submit((R)function.apply(item));  
		subscription.request(1);  
	}  
  
	@Override  
	public void onError(Throwable t) {  
		t.printStackTrace();  
	}  
  
	@Override  
	public void onComplete() {  
		close();  
	}  
}  
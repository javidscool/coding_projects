// Demonstrate a list view.
import javafx.application.*;
import javafx.scene.*;
import javafx.stage.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;
import javafx.geometry.*;
import javafx.beans.value.*;
import javafx.collections.*;

public class ListViewDemo extends Application {
	
	Label response;

	public static void main(String[] args) {
		// Start the JavaFX application by calling launch().
		launch(args);
	}
	
	// Override the start() method.
	public void start(Stage myStage) {
		// Give the stage a title.
		myStage.setTitle("ListView Demo");
		// Use a FlowPane for the root node. In this case,
		// vertical and horizontal gaps of 10.
		FlowPane rootNode = new FlowPane(10, 10);
		// Center the controls in the scene.
		rootNode.setAlignment(Pos.CENTER);
		// Create a scene.
		Scene myScene = new Scene(rootNode, 200, 120);
		// Set the scene on the stage.
		myStage.setScene(myScene);
		// Create a label.
		response = new Label("Select Transport Type");
	
		// Create an ObservableList of entries for the list view.
		ObservableList<String> transportTypes = FXCollections.observableArrayList( "Train", "Car", "Airplane", "Bicycle", "Walking" );
		// Create the list view.
		final ListView<String> lvTransport = new ListView<String>(transportTypes);
		lvTransport.getSelectionModel().setSelectionMode(SelectionMode.MULTIPLE);
		// Set the preferred height and width.
		lvTransport.setPrefSize(100, 80);
		// Get the list view selection model.
		MultipleSelectionModel<String> lvSelModel = lvTransport.getSelectionModel();
		// Use a change listener to respond to a change of selection within
		// a list view.
		lvSelModel.selectedItemProperty().addListener(
				new ChangeListener<String>() {
					public void changed(ObservableValue<? extends String> changed,
							String oldVal, String newVal) {
						String selItems = "";
						ObservableList<String> selected =
								lvTransport.getSelectionModel().getSelectedItems();
						// Display the selections.
						for(int i=0; i < selected.size(); i++)
							selItems += "\n " + selected.get(i);
						response.setText("All transports selected: " + selItems);
					}
				});
	
		// Add the label and list view to the scene graph.
		rootNode.getChildren().addAll(lvTransport, response);
		// Show the stage and its scene.
		myStage.show();
	}
}
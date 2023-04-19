import javafx.scene.control.Button
import javafx.scene.control.ChoiceBox
import javafx.scene.layout.Pane
import javafx.scene.layout.VBox

class Filtering (controller: Controller, private val model: Model) : Pane(), IView {
    // List out our list of filtering choices.
    private val filtering_choices: ArrayList<String> = arrayListOf("All Courses", "CS Courses Only", "MATH Courses Only", "All Other")

    private val filtering = ChoiceBox<String>()

    // Set the approprate filtering choice value when model state updates
    override fun updateView() {
        filtering.value = model.filter_by
    }

    init {
        // UI settings
        filtering.items.addAll(filtering_choices)
        filtering.maxWidth = 150.0

        // Call the controller to update model state
        filtering.valueProperty().addListener { _, _, newValue ->
            controller.set_filter_by(newValue)
        }

        children.add(filtering)

        model.addView(this)
    }
}
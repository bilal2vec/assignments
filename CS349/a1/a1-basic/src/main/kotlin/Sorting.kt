import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.control.Button
import javafx.scene.control.ChoiceBox
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javax.swing.GroupLayout.Alignment

class Sorting(controller: Controller, private val model: Model) : Pane(), IView {
    // Set our list of sorting
    private val sorting_choices: ArrayList<String> = arrayListOf("Course Code", "Term", "Grade (Asc)", "Grade (Desc)")

    private val sorting = ChoiceBox<String>()

    // Set the appropriate filtering choice when the model's state updates.
    override fun updateView() {
        sorting.value = model.sort_by
    }

    init {
        // UI settings
        sorting.items.addAll(sorting_choices)
        sorting.maxWidth = 150.0

        // Call the controller to update the model's sort by value
        sorting.valueProperty().addListener { _, _, newValue ->
            controller.set_sort_by(newValue)
        }

        children.add(sorting)

        model.addView(this)
    }
}
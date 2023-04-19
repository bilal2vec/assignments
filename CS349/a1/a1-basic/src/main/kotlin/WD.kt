import javafx.geometry.Insets
import javafx.scene.control.Button
import javafx.scene.control.CheckBox
import javafx.scene.layout.Pane
import javafx.scene.layout.VBox

class WD(controller: Controller, private val model: Model) : Pane(), IView {
    // Checkbox to exclude WD'ed courses
    private val wd = CheckBox("Exclude WD'ed Courses")

    // Update the Checkbox state if the model state changes
    override fun updateView() {
        wd.isSelected = model.hide_wd
    }

    init {
        wd.padding = Insets(4.0, 0.0, 0.0, 15.0)
        wd.isSelected = false

        // Call the controller to update the model state if the checkbox is toggled
        wd.selectedProperty().addListener { _, _, newValue ->
            controller.set_hide_wd(newValue)
        }

        children.add(wd)

        model.addView(this)
    }
}
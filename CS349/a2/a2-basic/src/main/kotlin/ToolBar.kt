import com.sun.javafx.geom.BaseBounds.BoundsType
import javafx.event.EventHandler
import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.control.Button
import javafx.scene.control.ChoiceBox
import javafx.scene.control.Label
import javafx.scene.control.TextArea
import javafx.scene.layout.*
import javafx.scene.paint.Color
import java.time.Duration
import javax.swing.GroupLayout.Alignment

class ToolBar(private val controller: Controller, private val model: Model) : HBox(), IView {

    private val course_code = TextArea()
    private val course_name = TextArea()
    private val term =  ChoiceBox<String>()
    private val grade =  TextArea()
    private val create_course = Button("Create")

    private val bottom_row = HBox()

    private val vbox = VBox(bottom_row)

    private val utils = Utils()

    // Toolbar doesn't use any state from the model, so this can be empty.
    override fun updateView() {
    }

    // Check to make sure the proposed course doesn't already exist and then vall the controller to create it
    private fun handle_create_course() {
        val course_code_exists = model.rows.any{it.course_code == course_code.text}
        if (course_code.text != "" && term.value != null && utils.valid_grade(grade.text) && !course_code_exists) {
            val new_course = Row(course_code.text, course_name.text, term.value, grade.text )

            course_code.text = ""
            course_name.text = ""
            term.value = null
            grade.text = ""

            controller.add_row(new_course)
        }
    }

    init {
        // UI settings

        course_code.maxWidth = 150.0
        course_code.minHeight = 30.0
        course_code.maxHeight = 30.0

        course_name.minHeight = 30.0
        course_name.maxHeight = 30.0

        term.items.addAll(listOf<String>("F20", "W21", "S21", "F21", "W22", "S22", "F22", "W23", "S23"))
        term.maxWidth = 150.0
        term.minHeight = 30.0
        term.maxHeight = 30.0

        grade.maxWidth = 150.0
        grade.minHeight = 30.0
        grade.maxHeight = 30.0

        // Create a handler to add a new course
        create_course.onAction = EventHandler { handle_create_course() }
        create_course.minWidth = 100.0
        create_course.minHeight = 30.0

        // Add parts of top bar as children to the scene
        bottom_row.children.add(course_code)
        bottom_row.children.add(term)
        bottom_row.children.add(grade)
        bottom_row.children.add(create_course)

        bottom_row.maxWidth = Double.MAX_VALUE
        bottom_row.padding = Insets(15.0, 15.0, 0.0, 15.0)
        HBox.setHgrow(course_name, Priority.ALWAYS)

        VBox.setVgrow(bottom_row, Priority.ALWAYS)

        children.add(vbox)
        HBox.setHgrow(vbox, Priority.ALWAYS)
        bottom_row.spacing = 15.0

        model.addView(this)
    }
}
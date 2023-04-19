import javafx.event.EventHandler
import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.Scene
import javafx.scene.control.*
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javafx.scene.shape.Rectangle
import javafx.stage.Screen

class Course (val row: Row, private val controller: Controller) : HBox() {
    // Initialize widgets to update and delete a course.
    private val course_code = Label()
    private val course_name = TextArea()
    private val term = ChoiceBox<String>()
    private val grade = TextArea()
    private val update = Button("Update")
    private val delete_or_undo = Button("Delete")

    private val utils = Utils()

    // Change Update and Delete button functionality when editing a course.
    private fun handle_edit_field() {
        if (term.value != row.term || grade.text != row.grade) {
            update.setDisable(false)
            delete_or_undo.text = "Undo"
        } else {
            update.setDisable(true)
            delete_or_undo.text = "Delete"
        }
    }

    // Update the Course with new values if it satisfies the requirements.
    private fun handle_update_course() {
        if (term.value != "" && utils.valid_grade(grade.text)) {
            update.setDisable(true)
            delete_or_undo.text = "Delete"

            val new_row = Row(row.course_code, course_name.text, term.value, grade.text)

            controller.update_row(row, new_row)
        } else {
            return
        }
    }

    // Handle clicking the Delete/Undo button and either Deleting the Course or Undoing any edits to it.
    private fun handle_delete_or_undo_course() {
        if (delete_or_undo.text == "Delete") {
            controller.delete_row(row)
        } else {
            course_name.text = row.course_name
            term.value = row.term
            grade.text = row.grade
            update.setDisable(true)
            delete_or_undo.text = "Delete"
        }
    }

    init {
        // Set UI properties
        course_code.minWidth = 100.0
        course_code.maxWidth = 100.0
        course_code.minHeight = 30.0
        course_code.maxHeight = 30.0
        course_code.text = row.course_code
        course_code.background = Background(BackgroundFill(Color.WHITE, CornerRadii.EMPTY, Insets.EMPTY))
        course_code.padding = Insets(5.0, 0.0, 5.0, 15.0)

        course_name.minWidth = 100.0
        course_name.maxWidth = Double.MAX_VALUE
        course_name.minHeight = 30.0
        course_name.maxHeight = 30.0
        course_name.text = row.course_name
        course_name.textProperty().addListener { _, _, _ ->
            handle_edit_field()
        }

        term.items.addAll(listOf<String>("F20", "W21", "S21", "F21", "W22", "S22", "F22", "W23", "S23"))
        term.minWidth = 50.0
        term.maxWidth = 50.0
        term.value = row.term
        term.selectionModel.selectedItemProperty().addListener { _, _, _ ->
            handle_edit_field()
        }

        grade.minWidth = 50.0
        grade.maxWidth = 50.0
        grade.minHeight = 30.0
        grade.maxHeight = 30.0
        grade.text = row.grade
        grade.textProperty().addListener { _, _, _ ->
            handle_edit_field()
        }

        update.minWidth = 75.0
        update.maxWidth = 75.0
        update.setDisable(true)
        update.onAction = EventHandler { handle_update_course() }

        delete_or_undo.minWidth = 75.0
        delete_or_undo.maxWidth = 75.0
        delete_or_undo.onAction = EventHandler { handle_delete_or_undo_course() }
        children.addAll(course_code, term, grade, update, delete_or_undo)

        HBox.setHgrow(course_name, Priority.ALWAYS)

        spacing = 5.0


    }
}
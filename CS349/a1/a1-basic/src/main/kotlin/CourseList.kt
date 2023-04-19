import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.Scene
import javafx.scene.control.*
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javafx.scene.shape.Rectangle
import javafx.stage.Screen

class CourseList (private val controller: Controller, private val model: Model) : VBox(), IView {
    // Create a list of courses to show
    private var course_list: VBox = VBox()

    private val utils = Utils()

    // Helper function to replace WD grades with zeros when sorting courses by grade
    private fun handle_wd_grades(grade: String): Double {
        if (grade == "WD") {
            return 0.0
        } else {
            return grade.toDouble()
        }
    }


    // Return the appropriate background color for each Course based on the grade
    private fun get_background_color(grade: String): Color {
        if (grade == "WD") {
            return Color.DARKSLATEGRAY
        } else {
            val grade_num = grade.toDouble()

            if (grade_num >= 0 && grade_num < 50) {
                return Color.LIGHTCORAL
            } else if (grade_num >= 50 && grade_num < 60) {
                return Color.LIGHTBLUE
            } else if (grade_num >= 60 && grade_num < 91) {
                return Color.LIGHTGREEN
            } else if (grade_num >= 91 && grade_num < 96) {
                return Color.SILVER
            } else if (grade_num >= 96 && grade_num < 101) {
                return Color.GOLD
            } else {
                return Color.WHITE
            }
        }
    }

    // Function to sort courses by a particular attribute before listing them.
    private fun sort_rows(rows: List<Row>, sort_by: String): List<Row> {
        if (sort_by == "Course Code") {
            return rows.sortedBy { it.course_code }
        } else if (sort_by == "Term") {
            return rows.sortedBy { it.term }
        } else if (sort_by == "Grade (Asc)") {
            return rows.sortedBy { handle_wd_grades(it.grade) }
        } else if (sort_by == "Grade (Desc)") {
            val sorted_rows =  rows.sortedBy { handle_wd_grades(it.grade) }
            return sorted_rows.reversed()
        }
        return rows
    }

    override fun updateView() {
        course_list = VBox()

        // Filter out WD'ed courses if appropriate
        var wd_filtered_rows = model.rows.filter { true }
        if (model.hide_wd) {
            wd_filtered_rows = wd_filtered_rows.filter { it.grade != "WD" }
        }

        // Filter courses by our chosen attribute
        val filtered_rows = utils.filter_rows(wd_filtered_rows, model.filter_by)

        // Sort courses by our chosen attribute
        val sorted_rows = sort_rows(filtered_rows, model.sort_by)

        // List out all courses.
        for (row in sorted_rows) {
            val course = Course(row, controller)
            val background_color = get_background_color(row.grade)

            course.background = Background(BackgroundFill(background_color, CornerRadii.EMPTY, Insets.EMPTY))
            course.padding = Insets(15.0, 15.0, 15.0, 15.0)

            course_list.children.add(course)
        }

        // Clear all courses already displayed when displaying course to prevent duplicates.
        getChildren().clear()
        children.add(course_list)
    }

    init {
        model.addView(this)
    }
}
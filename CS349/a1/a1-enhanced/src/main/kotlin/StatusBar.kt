import javafx.geometry.Insets
import javafx.scene.control.Button
import javafx.scene.control.Label
import javafx.scene.layout.HBox
import javafx.scene.layout.Pane
import javafx.scene.layout.Priority
import kotlin.math.max

class StatusBar(controller: Controller, private val model: Model): Pane(), IView {
    // Set labels to show Status Bar information
    private val avg = Label()
    private val stddev = Label()
    private val n_taken = Label()
    private val n_failed = Label()
    private val n_wd = Label()

    private val status_bar = HBox(avg, stddev, n_taken, n_failed, n_wd)

    private val utils = Utils()

    // Compute the grade statistics for all visible courses in the list
    override fun updateView() {

        // Get currently visible rows of classes
        val rows = utils.filter_rows(model.rows, model.filter_by)

        var grades = 0.0
        var max_grades = 0.0
        var grades_squared = 0.0
        var courses_count = 0
        var failed_courses_count = 0
        var wd_courses_count = 0

        for (row in rows) {
            if (row.grade == "WD") {
                wd_courses_count += 1
            } else {
                val grade = row.grade.toDouble()

                if (grade < 50) {
                    failed_courses_count += 1
                }

                grades += grade
                max_grades += 100
                grades_squared += grade * grade
                courses_count += 1
            }
        }

        val standard_deviation = Math.sqrt((grades_squared / courses_count) - ((grades / courses_count) * (grades / courses_count)))

        // Update the ui with the grade statistics information
        avg.text = "Course Average: " + ((grades / max_grades) * 100).toString().take(4)

        stddev.text = "Stddev: " + standard_deviation.toString().take(4)

        n_taken.text = "Courses Taken: " + courses_count.toString()
        n_failed.text = "Courses Failed: " + failed_courses_count.toString()

        // Only show the number of WD'ed courses if WD'ed courses are not hidden.
        if (model.hide_wd) {
            n_wd.text = ""
        } else {
            n_wd.text = "Courses WD'ed: " + wd_courses_count
        }
    }

    init {
        // UI setting
        avg.padding = Insets(10.0, 15.0, 0.0, 15.0)
        stddev.padding = Insets(10.0, 15.0, 0.0, 15.0)
        n_taken.padding = Insets(10.0, 15.0, 0.0, 15.0)
        n_failed.padding = Insets(10.0, 15.0, 0.0, 15.0)
        n_wd.padding = Insets(10.0, 15.0, 0.0, 15.0)

        children.add(status_bar)
        HBox.setHgrow(status_bar, Priority.ALWAYS)

        model.addView(this)
    }
}
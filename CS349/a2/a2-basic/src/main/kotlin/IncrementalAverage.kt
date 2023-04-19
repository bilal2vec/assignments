import javafx.event.EventHandler
import javafx.geometry.Insets
import javafx.scene.canvas.Canvas
import javafx.scene.control.*
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javafx.scene.text.Font
import kotlin.math.sqrt
import kotlin.random.Random

class IncrementalAverage(private val controller: Controller, private val model: Model) : Pane(), IView {
    // Add Panes for the top bar
    var canvas = Canvas(320.0, 240.0)

    var h = 500.0
    var w = 500.0

    override fun updateView() {
        draw()
    }

    private fun get_background_color(grade: String): Color {
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

    fun draw() {
        val margin = 30.0

        val height = this.height
        val width = this.width

        getChildren().clear()

        canvas = Canvas(width, height)
        children.add(canvas)

        // y-axis line
        canvas.graphicsContext2D.apply {
            stroke = Color.BLACK
            lineWidth = 2.0
            strokeLine(margin, 0.0, margin, height - margin)
        }

        // y-axis labels
        for (i in 0..10) {
            canvas.graphicsContext2D.apply {
                font = Font.font("Console", 12.0)
                fillText((100 - i * 10).toString(), 0.0, (height / 11) * i + 10)
            }
        }

        // y-axis lines
        for (i in 0..9) {
            canvas.graphicsContext2D.apply {
                stroke = Color.GRAY
                lineWidth = 1.0
                strokeLine(margin, (height / 11) * i + 10, width, (height / 11) * i + 10)
            }
        }

        // x-axis line
        canvas.graphicsContext2D.apply {
            stroke = Color.BLACK
            lineWidth = 2.0
            strokeLine(margin, height - margin, width, height - margin)
        }

        // Filter out WD'ed courses
        var wd_filtered_rows = model.rows.filter { true }
        wd_filtered_rows = wd_filtered_rows.filter { it.grade != "WD" }

        // Get list of terms
        var dup_terms: MutableList<String> = mutableListOf()
        for (row in wd_filtered_rows) {
            dup_terms.add(row.term)
        }
        var terms = dup_terms.distinct()

        // Sort terms
        terms = terms.sorted()

        // x-axis labels
        for (i in 0..terms.size - 1) {
            canvas.graphicsContext2D.apply {
                font = Font.font("Console", 12.0)
                fillText(terms[i], ((width - 2 * margin) / 10) * i + 2 * margin, height)
            }
        }

        // Plot all points
        for (row in wd_filtered_rows) {
            val term = row.term
            val grade = row.grade

            val x_val = terms.indexOf(term)

            // Plot points cumulative across terms
            for (x in x_val..terms.size - 1) {
                val y_val = grade.toDouble()

                canvas.graphicsContext2D.apply {
                    stroke = get_background_color(grade)
                    strokeOval(((width - 2 * margin) / 10) * x + 2 * margin, (height / 11) * (10 - y_val / 10.0) + 5, 10.0, 10.0)
                }

            }
        }

        // Compute cumulative avg and stddev
        var running_grade_sum = 0.0
        var running_grade_squared_sum = 0.0
        var n = 0

        for (term in 0..terms.size - 1) {
            val term_courses = wd_filtered_rows.filter { it.term == terms[term] }

            for (course in term_courses) {
                running_grade_sum += course.grade.toDouble()
                running_grade_squared_sum += course.grade.toDouble() * course.grade.toDouble()
                n += 1
            }

            val current_mean = running_grade_sum / n
            val current_squared_mean = running_grade_squared_sum / n
            val current_stddev = sqrt(current_squared_mean - (current_mean * current_mean))

            // plot cumulative mean
            canvas.graphicsContext2D.apply {
                fill = get_background_color(current_mean.toString())
                fillOval(((width - 2 * margin) / 10) * term + 2 * margin, (height / 11) * (10 - current_mean / 10.0) + 5, 10.0, 10.0)
            }

            // stddev straight line
            canvas.graphicsContext2D.apply {
                stroke = Color.BLACK
                lineWidth = 2.0
                strokeLine(((width - 2 * margin) / 10) * term + 2 * margin + 5, (height / 11) * (10 - current_mean / 10.0) - current_stddev, ((width - 2 * margin) / 10) * term + 2 * margin + 5, (height / 11) * (10 - current_mean / 10.0) + current_stddev + 10 + 5)
            }

            // stddev perpendicular lines
            canvas.graphicsContext2D.apply {
                stroke = Color.BLACK
                lineWidth = 2.0
                strokeLine(((width - 2 * margin) / 10) * term + 2 * margin, (height / 11) * (10 - current_mean / 10.0) - current_stddev, ((width - 2 * margin) / 10) * term + 2 * margin + 10, (height / 11) * (10 - current_mean / 10.0) - current_stddev)
            }

            canvas.graphicsContext2D.apply {
                stroke = Color.BLACK
                lineWidth = 2.0
                strokeLine(((width - 2 * margin) / 10) * term + 2 * margin, (height / 11) * (10 - current_mean / 10.0) + current_stddev + 10 + 5, ((width - 2 * margin) / 10) * term + 2 * margin + 10, (height / 11) * (10 - current_mean / 10.0) + current_stddev + 10 + 5)
            }
        }
    }

    init {
        // UI settings

        // Resize dynamically
        apply {
            widthProperty().addListener { _, _, new_w ->
                draw()
            }
            heightProperty().addListener { _, _, new_h ->
                draw()
            }
        }

        padding = Insets(15.0, 0.0, 0.0, 15.0)
        model.addView(this)
    }
}
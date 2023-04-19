import javafx.event.EventHandler
import javafx.geometry.Insets
import javafx.scene.canvas.Canvas
import javafx.scene.control.*
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javafx.scene.text.Font
import kotlin.random.Random

class ProgressTowardsDegree(private val controller: Controller, private val model: Model) : Pane(), IView {
    // Add Panes for the top bar
    var canvas = Canvas(320.0, 240.0)

    var h = 500.0
    var w = 500.0

    override fun updateView() {
        draw()
    }

    fun draw() {
        val margin = 30.0

        val height = this.height
        val width = this.width

        getChildren().clear()

        canvas = Canvas(width, height)
        children.add(canvas)

        // y-axis labels
        val labels = listOf<String>("CS", "MATH", "Other", "ALL")
        for (i in 0..3) {
            canvas.graphicsContext2D.apply {
                font = Font.font("Console", 12.0)
                fillText(labels[i], 0.0, (height / 5) * i + (height / 5))
            }
        }

        // target bars
        val targets = listOf<Double>(11.0, 4.0, 5.0, 20.0)
        for (i in 0..3) {
            canvas.graphicsContext2D.apply {
                stroke = Color.BLACK
                fill = Color.BLACK
                fillRect( 2 * margin, (height / 5) * i + (height / 5), ((width - 4 * margin) / 5) * (targets[i] / 5), (height / 5) / 2)
            }
        }

        // x-axis lines
        for (i in 0..4) {
            canvas.graphicsContext2D.apply {
                stroke = Color.BLACK
                lineWidth = 2.0
                strokeLine( ((width - 4 * margin) / 5) * i + 2 * margin , margin, ((width - 4 * margin) / 5) * i + 2 * margin , height - margin)
            }
        }

        // x-axis labels
        for (i in 0..4) {
            canvas.graphicsContext2D.apply {
                font = Font.font("Console", 12.0)
                fillText((i * 5).toString(), ((width - 4 * margin) / 5) * i + 2 * margin, height)
            }
        }

        // Filter out WD'ed courses
        var wd_filtered_rows = model.rows.filter { true }
        wd_filtered_rows = wd_filtered_rows.filter { it.grade != "WD" }

        // Count credits for each course
        var n_cs = 0
        var n_math = 0
        var n_other = 0
        var n_all = 0

        for (row in wd_filtered_rows) {
            if ("CS" in row.course_code) {
                n_cs += 1
            } else if (("MATH" in row.course_code) || ("STAT" in row.course_code) || ("CO" in row.course_code)) {
                n_math += 1
            } else {
                n_other += 1
            }
            n_all += 1
        }

        // Plot bar for CS credits taken
        canvas.graphicsContext2D.apply {
            stroke = Color.BLUE
            fill = Color.BLUE
            fillRect( 2 * margin, (height / 5) * 0 + (height / 5), ((width - 4 * margin) / 5) * (n_cs / 5.0), (height / 5) / 2)
        }

        // Plot bar for MATH credits taken
        canvas.graphicsContext2D.apply {
            stroke = Color.BLUE
            fill = Color.BLUE
            fillRect( 2 * margin, (height / 5) * 1 + (height / 5), ((width - 4 * margin) / 5) * (n_math / 5.0), (height / 5) / 2)
        }

        // Plot bar for Other credits taken
        canvas.graphicsContext2D.apply {
            stroke = Color.BLUE
            fill = Color.BLUE
            fillRect( 2 * margin, (height / 5) * 2 + (height / 5), ((width - 4 * margin) / 5) * (n_other / 5.0), (height / 5) / 2)
        }

        // Plot bar for all credits taken
        canvas.graphicsContext2D.apply {
            stroke = Color.BLUE
            fill = Color.BLUE
            fillRect( 2 * margin, (height / 5) * 3 + (height / 5), ((width - 4 * margin) / 5) * (n_all / 5.0), (height / 5) / 2)
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
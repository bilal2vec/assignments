import javafx.event.EventHandler
import javafx.geometry.Insets
import javafx.scene.control.*
import javafx.scene.layout.*
import javafx.scene.paint.Color
import kotlin.random.Random

class Tabs(private val controller: Controller, private val model: Model) : HBox(), IView {
    // Add tabs
    val average_by_term = AverageByTerm(controller, model)
    val progress_towards_degree = ProgressTowardsDegree(controller, model)
    val course_outcomes = Button("Course Outcomes")
    val incremental_averages = IncrementalAverage(controller, model)

    val tabs = TabPane().apply {
        tabs.add(Tab("Average By Term", average_by_term))
        tabs.add(Tab("Progress Towards Degree", progress_towards_degree))
        tabs.add(Tab("Course Outcomes", course_outcomes))
        tabs.add(Tab("Incremental Averages", incremental_averages))
        tabClosingPolicy = TabPane.TabClosingPolicy.UNAVAILABLE
    }

    val hbox = HBox(tabs)
    val vbox = VBox(hbox)

    override fun updateView() {
    }

    init {
        // UI settings

        hbox.maxWidth = Double.MAX_VALUE
        hbox.padding = Insets(15.0, 15.0, 0.0, 15.0)
        HBox.setHgrow(tabs, Priority.ALWAYS)
        HBox.setHgrow(vbox, Priority.ALWAYS)

        VBox.setVgrow(hbox, Priority.ALWAYS)

        children.add(vbox)
        minWidth = 600.0

        model.addView(this)
    }
}
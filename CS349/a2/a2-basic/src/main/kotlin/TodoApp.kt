import javafx.application.Application
import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.Scene
import javafx.scene.control.Button
import javafx.scene.control.Label
import javafx.scene.control.ScrollPane
import javafx.stage.Stage
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javax.swing.text.LabelView

class TodoApp : Application() {
    override fun start(stage: Stage) {
        // Set title and initial window size
        stage.title = "CS349 - A2 My Mark Visualization - b54khan"

        stage.width = 900.0
        stage.height = 450.0
        stage.minWidth = 450.0
        stage.minHeight = 450.0

        // Initialize model and controller
        val model = Model()
        val controller = Controller(model)

        // Initialize main views
        val tool_bar = ToolBar(controller, model)
        tool_bar.minHeight = 100.0
        tool_bar.maxHeight = 100.0

        val courselist = CourseList(controller, model)
        courselist.minHeight = 300.0
        courselist.maxHeight = Double.MAX_VALUE

        val scroll = ScrollPane(courselist)
        scroll.hbarPolicy = ScrollPane.ScrollBarPolicy.ALWAYS
        scroll.isFitToWidth = true
        scroll.maxHeight = Double.MAX_VALUE

        val status_bar = StatusBar(controller, model)
        status_bar.minHeight = 50.0
        status_bar.maxHeight = 50.0

        val window = VBox(tool_bar, scroll)
        window.minWidth = 425.0
        window.maxWidth = 425.0

        VBox.setVgrow(tool_bar, Priority.NEVER)
        VBox.setVgrow(courselist, Priority.ALWAYS)

        val tabs = Tabs(controller, model)

        val viz_window = VBox(tabs)

        VBox.setVgrow(tabs, Priority.ALWAYS)

        val windows = HBox(window, viz_window)
        windows.maxHeight = Double.MAX_VALUE
        HBox.setHgrow(viz_window, Priority.ALWAYS)

        val page = VBox(windows, status_bar)
        VBox.setVgrow(status_bar, Priority.NEVER)
        VBox.setVgrow(windows, Priority.ALWAYS)

        stage.scene = Scene(page)
        stage.show()
    }
}
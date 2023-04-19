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
        stage.title = "CS349 - A1 My Mark Management - b54khan"

        stage.width = 800.0
        stage.height = 600.0
        stage.minWidth = 800.0
        stage.minHeight = 600.0

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

        val status_bar = StatusBar(controller, model)
        status_bar.minHeight = 50.0
        status_bar.maxHeight = 50.0

        val window = VBox(tool_bar, scroll, status_bar)

        VBox.setVgrow(tool_bar, Priority.NEVER)
        VBox.setVgrow(courselist, Priority.ALWAYS)
        VBox.setVgrow(status_bar, Priority.NEVER)

        stage.scene = Scene(window)
        stage.show()
    }
}
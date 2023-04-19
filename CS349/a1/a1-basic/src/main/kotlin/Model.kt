// From https://git.uwaterloo.ca/cs349/public/sample-code/-/blob/master/MVC/02.MVC1/src/main/kotlin/Model.kt

data class Row (val course_code: String, val course_name: String, val term: String, val grade: String)

class Model {
    // Store our list of courses
    val rows: ArrayList<Row> = ArrayList()

    // Store our sorting and filtering settings
    var sort_by: String = "Course Code"
    var filter_by: String = "All Courses"
    var hide_wd: Boolean = false

    // List of all views so we can notify them to update their values
    private val views: ArrayList<IView> = ArrayList()

    init {
        // Prepopulate the model with some courses
        val row = Row("CS 349", "UI", "W23", "10")
        val row2 = Row("CS 341", "Algo", "W23", "50" )
        val row3 = Row("MATH 213", "Diff Eq", "W23", "55" )
        val row4 = Row("STAT 206", "Stats", "W22", "70" )
        val row5 = Row("ENGL 108P", "Harry Potter", "W23", "95" )
        val row6 = Row("MATH 235", "Lin Alg 2", "F22", "WD" )
        val row7 = Row("MATH 239", "Combinatorics", "S22", "100" )

        rows.add(row)
        rows.add(row2)
        rows.add(row3)
        rows.add(row4)
        rows.add(row5)
        rows.add(row6)
        rows.add(row7)
    }

    // Register a new view and update it.
    fun addView(view: IView) {
        views.add(view)
        view.updateView()
    }

    // Notify and update all observer views of the model
    private fun notifyObservers() {
        for (view in views) {
            view.updateView()
        }
    }

    // Add a new row to the list of courses and notify all observers.
    fun add_row(r: Row) {
        rows.add(r)
        notifyObservers()
    }

    // Remove a row from our list of courses and notify all observers.
    fun remove_row(r: Row) {
        rows.remove(r)
        notifyObservers()
    }

    // Set the value we want to sort by and notify all observers.
    fun set_sort_by(value: String) {
        sort_by = value

        notifyObservers()
    }

    // Set the value we want to filter by and notify all observers.
    fun set_filter_by(value: String) {
        filter_by = value

        notifyObservers()
    }

    // Set if we want to hide WD'ed courses or not and notify all observers.
    fun set_hide_wd(value: Boolean) {
        hide_wd = value
        notifyObservers()
    }


}
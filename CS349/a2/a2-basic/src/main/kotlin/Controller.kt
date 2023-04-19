// From https://git.uwaterloo.ca/cs349/public/sample-code/-/blob/master/MVC/02.MVC1/src/main/kotlin/Controller.kt
class Controller(var model: Model) {

    // Implement functions to change (add, delete, update) courses and also change Model state on how to sort/filter the list of courses.

    fun add_row(r: Row) {
        model.add_row(r)
    }

    fun delete_row(r: Row) {
        model.remove_row(r)
    }

    fun update_row(r: Row, new_row: Row) {
        model.remove_row(r)
        model.add_row(new_row)
    }

    fun set_sort_by(sort_by: String) {
        model.set_sort_by(sort_by)
    }

    fun set_filter_by(filter_by: String) {
        model.set_filter_by(filter_by)
    }

    fun set_hide_wd(hide_wd: Boolean) {
        model.set_hide_wd(hide_wd)
    }
}

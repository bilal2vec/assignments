import android.util.Log
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

data class Row (val course_code: String, val course_name: String, val term: String, val grade: String)

class MyViewModel : ViewModel() {
    private var rows = MutableLiveData<ArrayList<Row>>()

    // Store our sorting and filtering settings
    var sort_by = MutableLiveData<String>("com.example.mymobilemarkmanagement.Course Code")
    var filter_by = MutableLiveData<String>("All Courses")

    var current_course = Row("", "", "", "")

    fun add_course(row: Row) {
        if (rows.value == null) {
            rows.value = arrayListOf(row)
        }
        else {
            var list = rows.value!!
            list.add(row)
            rows.value = list

        }
    }

    init {
        // Prepopulate the model with some courses
        val row = Row("CS 349", "UI", "W23", "10")
        val row2 = Row("CS 341", "Algo", "W23", "50" )
        val row3 = Row("MATH 213", "Diff Eq", "W23", "55" )
        val row4 = Row("STAT 206", "Stats", "W22", "70" )
        val row5 = Row("ENGL 108P", "Harry Potter", "W23", "95" )
        val row6 = Row("MATH 235", "Lin Alg 2", "F22", "WD" )
        val row7 = Row("MATH 239", "Combinatorics", "S22", "100" )

        add_course(row)
        add_course(row2)
        add_course(row3)
        add_course(row4)
        add_course(row5)
        add_course(row6)
        add_course(row7)

    }

    fun getRows(): LiveData<ArrayList<Row>> {
        return rows
    }

    fun get_sort_by(): LiveData<String> {
        return sort_by
    }

    fun get_filter_by(): LiveData<String> {
        return filter_by
    }

    fun add_row(r: Row) {
        add_course(r)
    }

    fun remove_row(r: Row) {
        if (rows.value != null) {
            var list = rows.value!!

            list.remove(r)

            rows.value = list
        }
    }

    fun set_sort_by(value: String) {
        sort_by.value = value
    }

    fun set_filter_by(value: String) {
        filter_by.value = value
    }

    fun set_current_course(r:Row) {
        current_course = r
    }

    fun get_current_course() : Row {
        return current_course
    }
}
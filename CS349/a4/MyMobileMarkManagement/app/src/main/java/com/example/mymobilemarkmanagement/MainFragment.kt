package com.example.mymobilemarkmanagement

import MyViewModel
import Row
import android.graphics.Color
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.fragment.findNavController

fun get_background_color(grade: String): Int {
    if (grade == "WD") {
        return Color.parseColor("#2F4F4F")
    } else {
        val grade_num = grade.toDouble()

        if (grade_num >= 0 && grade_num < 50) {
            return Color.parseColor("#F08080")
        } else if (grade_num >= 50 && grade_num < 60) {
            return Color.parseColor("#ADD8E6")
        } else if (grade_num >= 60 && grade_num < 91) {
            return Color.parseColor("#90EE90")
        } else if (grade_num >= 91 && grade_num < 96) {
            return Color.parseColor("#C0C0C0")
        } else if (grade_num >= 96 && grade_num < 101) {
            return Color.parseColor("#FFD700")
        } else {
            return Color.parseColor("#FFD700")
        }
    }
}

class MainFragment : Fragment() {

    // Helper function to replace WD grades with zeros when sorting courses by grade
    private fun handle_wd_grades(grade: String): Double {
        if (grade == "WD") {
            return 0.0
        } else {
            return grade.toDouble()
        }
    }

    // Function to filter courses by particular attributes to only show particular types of courses
    fun filter_rows(rows: List<Row>, filter_by: String): List<Row> {
        if (filter_by == "All Courses") {
            return rows
        } else if (filter_by == "CS Only") {
            return rows.filter { "CS" in it.course_code }
        } else if (filter_by == "Math Only") {
            return rows.filter { "MATH" in it.course_code || "STAT" in it.course_code || "CO" in it.course_code }
        } else if (filter_by == "Other Only") {
            return rows.filter { ! ("CS" in it.course_code || "MATH" in it.course_code || "STAT" in it.course_code || "CO" in it.course_code ) }
        }

        return rows
    }

    // Function to sort courses by a particular attribute before listing them.
    private fun sort_rows(rows: List<Row>, sort_by: String): List<Row> {
        if (sort_by == "By Course Code") {
            return rows.sortedBy { it.course_code }
        } else if (sort_by == "By Term") {
            return rows.sortedBy { it.term }
        } else if (sort_by == "By Mark") {
            val sorted_rows =  rows.sortedBy { handle_wd_grades(it.grade) }
            return sorted_rows.reversed()
        }
        return rows
    }

    fun filter_and_sort_rows(rows: ArrayList<Row>, filter_by_value: String, sort_by_value: String) : List<Row> {
        // Filter out WD'ed courses if appropriate
        var wd_filtered_rows = rows.filter { true }

        // Filter courses by our chosen attribute
        val filtered_rows = filter_rows(wd_filtered_rows, filter_by_value)

        // Sort courses by our chosen attribute
        val sorted_rows = sort_rows(filtered_rows, sort_by_value)

        return sorted_rows
    }
    fun addElement(parent: LinearLayout, row: Row) {
        var course = layoutInflater.inflate(R.layout.course, parent, false)
        val myVM = ViewModelProvider(requireActivity())[MyViewModel::class.java]

        course.findViewById<TextView>(R.id.course_code).text = row.course_code
        course.findViewById<TextView>(R.id.term).text = row.term
        course.findViewById<TextView>(R.id.grade).text = row.grade

        course.findViewById<TextView>(R.id.course_name).text = row.course_name

        course.setBackgroundColor(get_background_color(row.grade))

        course.findViewById<Button>(R.id.edit_button).apply {
            setOnClickListener {

                myVM.set_current_course(row)

                findNavController().navigate(R.id.edit_fragment)
            }
        }

        course.findViewById<Button>(R.id.delete_button).apply {
            setOnClickListener {
                myVM.remove_row(row)
            }
        }


        parent.addView(course)
    }

    fun add_rows(parent: LinearLayout, filter_by_value: String, sort_by_value: String,  rows: ArrayList<Row>?) {
        if (rows != null) {
            val rows_prime = filter_and_sort_rows(rows, filter_by_value, sort_by_value)
            for (row in rows_prime) {
                addElement(parent, row)
            }
        }
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {

        val root = inflater.inflate(R.layout.fragment_main, container, false)
        val myVM = ViewModelProvider(requireActivity())[MyViewModel::class.java]

        val filterSpinner = root.findViewById<Spinner>(R.id.filter_spinner)
        val filterAdapter = ArrayAdapter.createFromResource(requireContext(),
            R.array.filter_options, android.R.layout.simple_spinner_item)
        filterAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        filterSpinner.adapter = filterAdapter

        val sortSpinner = root.findViewById<Spinner>(R.id.sorting_spinner)
        val sortAdapter = ArrayAdapter.createFromResource(requireContext(),
            R.array.sort_options, android.R.layout.simple_spinner_item)
        sortAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        sortSpinner.adapter = sortAdapter

        root.findViewById<Button>(R.id.add_button).apply {
            setOnClickListener {
                findNavController().navigate(R.id.addFragment)
            }
        }

        filterSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(parent: AdapterView<*>, view: View, position: Int, id: Long) {
                myVM.set_filter_by(parent.getItemAtPosition(position).toString())
            }

            override fun onNothingSelected(parent: AdapterView<*>) {
            }
        }

        sortSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(parent: AdapterView<*>, view: View, position: Int, id: Long) {
                myVM.set_sort_by(parent.getItemAtPosition(position).toString())
            }

            override fun onNothingSelected(parent: AdapterView<*>) {
            }
        }

        val linearLayout = root.findViewById<LinearLayout>(R.id.linear_layout)

        myVM.get_filter_by().observe(viewLifecycleOwner) {
            root.findViewById<Spinner>(R.id.filter_spinner).setSelection(filterAdapter.getPosition(it))

            linearLayout.removeAllViews()

            val filter_by_value = myVM.filter_by.value!!
            val sort_by_value = myVM.sort_by.value!!
            val rows = myVM.getRows().value

            add_rows(linearLayout, filter_by_value, sort_by_value, rows)
        }

        myVM.get_sort_by().observe(viewLifecycleOwner) {
            root.findViewById<Spinner>(R.id.sorting_spinner).setSelection(sortAdapter.getPosition(it))

            linearLayout.removeAllViews()

            val filter_by_value = myVM.filter_by.value!!
            val sort_by_value = myVM.sort_by.value!!
            val rows = myVM.getRows().value

            add_rows(linearLayout, filter_by_value, sort_by_value, rows)
        }


        myVM.getRows().observe(viewLifecycleOwner) {
            linearLayout.removeAllViews()

            val filter_by_value = myVM.filter_by.value!!
            val sort_by_value = myVM.sort_by.value!!
            val rows = myVM.getRows().value

            add_rows(linearLayout, filter_by_value, sort_by_value, rows)
        }

        return root
    }
}
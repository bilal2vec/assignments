package com.example.mymobilemarkmanagement

import MyViewModel
import Row
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.CheckBox
import android.widget.EditText
import android.widget.Spinner
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.fragment.findNavController

class AddFragment : Fragment() {
    var is_disabled : Boolean = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    fun add_course(root: View, model: MyViewModel) : Boolean {
        val course_code_input = root.findViewById<EditText>(R.id.course_code_input).text.toString()
        val grade_input = root.findViewById<EditText>(R.id.grade_input).text.toString()
        val checkbox_input = root.findViewById<CheckBox>(R.id.checkbox_input).isChecked
        val term_input = root.findViewById<EditText>(R.id.term_input).text.toString()
        val course_name_input = root.findViewById<EditText>(R.id.course_name_input).text.toString()

        var grade = grade_input
        if (checkbox_input) {
            grade = "WD"
        }

        if (course_code_input != "" && grade != "" && term_input != "") {
            val row = Row(course_code_input, course_name_input, term_input, grade)
            model.add_row(row)

            return true
        }

        return false
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root =  inflater.inflate(R.layout.fragment_add, container, false)
        val myVM = ViewModelProvider(requireActivity())[MyViewModel::class.java]

        root.findViewById<Button>(R.id.add_course_button).apply {
            setOnClickListener {

                if(add_course(root, myVM)) {
                    findNavController().navigate(R.id.mainFragment)
                }

            }
        }

        root.findViewById<Button>(R.id.cancel_add_course_button).apply {
            setOnClickListener {
                findNavController().navigate(R.id.mainFragment)
            }
        }

        root.findViewById<CheckBox>(R.id.checkbox_input).apply {
            setOnClickListener {
                if (is_disabled) {
                    root.findViewById<EditText>(R.id.grade_input).isEnabled = true
                    root.findViewById<EditText>(R.id.grade_input).setText("")
                    is_disabled = false
                } else {
                    root.findViewById<EditText>(R.id.grade_input).isEnabled = false
                    root.findViewById<EditText>(R.id.grade_input).setText("")
                    is_disabled = true
                }
            }
        }



        return root
    }

}
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

class EditFragment : Fragment() {
    var is_disabled : Boolean = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    fun edit_course(root: View, model: MyViewModel) : Boolean {
        val course_code_input = root.findViewById<EditText>(R.id.course_code_input_edit).text.toString()
        val grade_input = root.findViewById<EditText>(R.id.grade_input_edit).text.toString()
        val checkbox_input = root.findViewById<CheckBox>(R.id.checkbox_input_edit).isChecked
        val term_input = root.findViewById<EditText>(R.id.term_input_edit).text.toString()
        val course_name_input = root.findViewById<EditText>(R.id.course_name_input_edit).text.toString()

        var grade = grade_input
        if (checkbox_input) {
            grade = "WD"
        }

        if (grade != "" && term_input != "") {
            val original_row = model.get_current_course()
            model.remove_row(original_row)
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
        val root =  inflater.inflate(R.layout.fragment_edit, container, false)
        val myVM = ViewModelProvider(requireActivity())[MyViewModel::class.java]

        root.findViewById<EditText>(R.id.course_code_input_edit).setText(myVM.get_current_course().course_code)
        root.findViewById<EditText>(R.id.course_code_input_edit).isEnabled = false
        root.findViewById<EditText>(R.id.course_name_input_edit).setText(myVM.get_current_course().course_name)
        root.findViewById<EditText>(R.id.term_input_edit).setText(myVM.get_current_course().term)
        root.findViewById<EditText>(R.id.grade_input_edit).setText(myVM.get_current_course().grade)


        if (myVM.get_current_course().grade == "WD") {
            root.findViewById<EditText>(R.id.grade_input_edit).isEnabled = false
            is_disabled = true
            root.findViewById<CheckBox>(R.id.checkbox_input_edit).isChecked = true

        } else {
            root.findViewById<EditText>(R.id.grade_input_edit).setText(myVM.get_current_course().grade)
        }


        root.findViewById<Button>(R.id.edit_course_button).apply {
            setOnClickListener {

                if(edit_course(root, myVM)) {
                    findNavController().navigate(R.id.mainFragment)
                }

            }
        }

        root.findViewById<Button>(R.id.cancel_edit_course_button).apply {
            setOnClickListener {
                findNavController().navigate(R.id.mainFragment)
            }
        }

        root.findViewById<CheckBox>(R.id.checkbox_input_edit).apply {
            setOnClickListener {
                if (is_disabled) {
                    root.findViewById<EditText>(R.id.grade_input_edit).isEnabled = true
                    if (myVM.get_current_course().grade != "WD") {
                        root.findViewById<EditText>(R.id.grade_input_edit).setText(myVM.get_current_course().grade)
                    } else {
                        root.findViewById<EditText>(R.id.grade_input_edit).setText("")
                    }
                    is_disabled = false
                } else {
                    root.findViewById<EditText>(R.id.grade_input_edit).isEnabled = false
                    root.findViewById<EditText>(R.id.grade_input_edit).setText("")
                    is_disabled = true
                }
            }
        }



        return root
    }

}
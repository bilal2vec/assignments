// Util class to store functions we want to reuse
class Utils {

    // Function to filter courses by particular attributes to only show particular types of courses
    fun filter_rows(rows: List<Row>, filter_by: String): List<Row> {
        if (filter_by == "All Courses") {
            return rows
        } else if (filter_by == "CS Courses Only") {
            return rows.filter { "CS" in it.course_code }
        } else if (filter_by == "MATH Courses Only") {
            return rows.filter { "MATH" in it.course_code || "STAT" in it.course_code || "CO" in it.course_code }
        } else if (filter_by == "All Other") {
            return rows.filter { ! ("CS" in it.course_code || "MATH" in it.course_code || "STAT" in it.course_code || "CO" in it.course_code ) }
        }

        return rows
    }

    // Check if a grade is a valid grade
    fun valid_grade(grade: String): Boolean {
        if (grade == "WD") {
            return true
        } else {
            val grade_num = grade.toDoubleOrNull()

            if (grade_num != null) {
                if (grade_num >= 0 && grade_num <= 100) {
                    return true
                } else {
                    return false
                }
            } else {
                return false
            }
        }
    }

}
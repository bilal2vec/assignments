static char sqla_program_id[292] = 
{
 '\xac','\x0','\x41','\x45','\x41','\x56','\x41','\x49','\x30','\x41','\x32','\x79','\x52','\x5a','\x47','\x6d','\x30','\x31','\x31','\x31',
 '\x31','\x20','\x32','\x20','\x20','\x20','\x20','\x20','\x20','\x20','\x20','\x20','\x8','\x0','\x42','\x35','\x34','\x4b','\x48','\x41',
 '\x4e','\x20','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x8','\x0','\x45','\x4e','\x52','\x4f','\x4c','\x4c','\x20','\x20','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0'
};

#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};


static const short sqlIsLiteral   = SQL_IS_LITERAL;
static const short sqlIsInputHvar = SQL_IS_INPUT_HVAR;


#line 1 "enroll.sqc"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"


/*
EXEC SQL INCLUDE SQLCA;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 6 "enroll.sqc"


int main(int argc, char *argv[])
{
    
/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "enroll.sqc"

    char db[6] = "cs348";

    sqlint32 id;
    char activity_type;
    char day[11];
    char time[9];
    char cnum[6];
    sqlint32 section;

    sqlint32 temp;
    char temp_str[20];

    
/*
EXEC SQL END DECLARE SECTION;
*/

#line 23 "enroll.sqc"


    id = atoi(argv[1]);
    activity_type = argv[2][0];
    strncpy(day, argv[3], 10);
    strncpy(time, argv[4], 9);
    strncpy(cnum, argv[5], 6);

    if (argc == 7)
    {
        section = atoi(argv[6]);
    }
    else
    {
        section = -1;
    }

    
/*
EXEC SQL WHENEVER SQLERROR GO TO error;
*/

#line 40 "enroll.sqc"

    
/*
EXEC SQL CONNECT TO : db;
*/

{
#line 41 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 41 "enroll.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 41 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 41 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)db;
#line 41 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 41 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 41 "enroll.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 41 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 41 "enroll.sqc"
  sqlastop(0L);
}

#line 41 "enroll.sqc"


    
/*
EXEC SQL WHENEVER NOT FOUND GO TO error_no_student_professor;
*/

#line 43 "enroll.sqc"

    
/*
EXEC SQL select id into : temp from((select professor.pnum as id from professor) union(select student.snum as id from student)) as ids where ids.id = : id;
*/

{
#line 44 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 44 "enroll.sqc"
  sqlaaloc(2,1,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 44 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 44 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 44 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 44 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 44 "enroll.sqc"
  sqlaaloc(3,1,3,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 44 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 44 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 44 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 44 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 44 "enroll.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 44 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 44 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto error_no_student_professor;
  }
#line 44 "enroll.sqc"
  sqlastop(0L);
}

#line 44 "enroll.sqc"


    if (activity_type == 'T')
    {
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO error_assign_lec_to_non_prof;
*/

#line 48 "enroll.sqc"

        
/*
EXEC SQL select pnum into : temp from professor where professor.pnum = : id;
*/

{
#line 49 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 49 "enroll.sqc"
  sqlaaloc(2,1,4,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 49 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 49 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 49 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 49 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 49 "enroll.sqc"
  sqlaaloc(3,1,5,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 49 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 49 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 49 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 49 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 49 "enroll.sqc"
  sqlacall((unsigned short)24,2,2,3,0L);
#line 49 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 49 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto error_assign_lec_to_non_prof;
  }
#line 49 "enroll.sqc"
  sqlastop(0L);
}

#line 49 "enroll.sqc"

    }

    if (section != -1)
    {
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO section_diff_time_end;
*/

#line 54 "enroll.sqc"

        
/*
EXEC SQL select section into : temp from section where exists(select time, day from section where cnum =
                                                                          : cnum and term = (select distinct term from enrollment where grade is null) and section =
                                                                              : section) and section = : section and (day !=
                                                                                                                      : day or time !=
                                                                                                                      : time);
*/

{
#line 59 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 59 "enroll.sqc"
  sqlaaloc(2,5,6,0L);
    {
      struct sqla_setdata_list sql_setdlist[5];
#line 59 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 59 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 59 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 59 "enroll.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 59 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)&section;
#line 59 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 59 "enroll.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 59 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)&section;
#line 59 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 59 "enroll.sqc"
      sql_setdlist[3].sqltype = 460; sql_setdlist[3].sqllen = 11;
#line 59 "enroll.sqc"
      sql_setdlist[3].sqldata = (void*)day;
#line 59 "enroll.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 59 "enroll.sqc"
      sql_setdlist[4].sqltype = 460; sql_setdlist[4].sqllen = 9;
#line 59 "enroll.sqc"
      sql_setdlist[4].sqldata = (void*)time;
#line 59 "enroll.sqc"
      sql_setdlist[4].sqlind = 0L;
#line 59 "enroll.sqc"
      sqlasetdata(2,0,5,sql_setdlist,0L,0L);
    }
#line 59 "enroll.sqc"
  sqlaaloc(3,1,7,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 59 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 59 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 59 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 59 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 59 "enroll.sqc"
  sqlacall((unsigned short)24,3,2,3,0L);
#line 59 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 59 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto section_diff_time_end;
  }
#line 59 "enroll.sqc"
  sqlastop(0L);
}

#line 59 "enroll.sqc"

        printf("Error: this section exists at a different time\n");
        goto error;
    }
section_diff_time_end:

    if (activity_type == 'O')
    {
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO error_assign_oh_to_non_prof;
*/

#line 67 "enroll.sqc"

        
/*
EXEC SQL select pnum into : temp from professor where professor.pnum = : id;
*/

{
#line 68 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 68 "enroll.sqc"
  sqlaaloc(2,1,8,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 68 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 68 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 68 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 68 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 68 "enroll.sqc"
  sqlaaloc(3,1,9,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 68 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 68 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 68 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 68 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 68 "enroll.sqc"
  sqlacall((unsigned short)24,4,2,3,0L);
#line 68 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 68 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto error_assign_oh_to_non_prof;
  }
#line 68 "enroll.sqc"
  sqlastop(0L);
}

#line 68 "enroll.sqc"


        
/*
EXEC SQL WHENEVER NOT FOUND GO TO error_oh_for_unknown_class;
*/

#line 70 "enroll.sqc"

        
/*
EXEC SQL select cname into : temp_str from course where cnum = : cnum;
*/

{
#line 71 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 71 "enroll.sqc"
  sqlaaloc(2,1,10,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 71 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 71 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 71 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 71 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 71 "enroll.sqc"
  sqlaaloc(3,1,11,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 71 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 20;
#line 71 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)temp_str;
#line 71 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 71 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 71 "enroll.sqc"
  sqlacall((unsigned short)24,5,2,3,0L);
#line 71 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 71 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto error_oh_for_unknown_class;
  }
#line 71 "enroll.sqc"
  sqlastop(0L);
}

#line 71 "enroll.sqc"

    }

    if (activity_type == 'C')
    {
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO error_enrolling_non_student;
*/

#line 76 "enroll.sqc"

        
/*
EXEC SQL select year into : temp from student where snum = : id;
*/

{
#line 77 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 77 "enroll.sqc"
  sqlaaloc(2,1,12,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 77 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 77 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 77 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 77 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 77 "enroll.sqc"
  sqlaaloc(3,1,13,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 77 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 77 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 77 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 77 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 77 "enroll.sqc"
  sqlacall((unsigned short)24,6,2,3,0L);
#line 77 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 77 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto error_enrolling_non_student;
  }
#line 77 "enroll.sqc"
  sqlastop(0L);
}

#line 77 "enroll.sqc"

    }

    
/*
EXEC SQL WHENEVER NOT FOUND GO TO error_class_doesnt_exist;
*/

#line 80 "enroll.sqc"

    
/*
EXEC SQL select cname into : temp_str from course where cnum = : cnum;
*/

{
#line 81 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 81 "enroll.sqc"
  sqlaaloc(2,1,14,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 81 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 81 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 81 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 81 "enroll.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 81 "enroll.sqc"
  sqlaaloc(3,1,15,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 81 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 20;
#line 81 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)temp_str;
#line 81 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 81 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 81 "enroll.sqc"
  sqlacall((unsigned short)24,7,2,3,0L);
#line 81 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 81 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto error_class_doesnt_exist;
  }
#line 81 "enroll.sqc"
  sqlastop(0L);
}

#line 81 "enroll.sqc"


    if (activity_type == 'T')
    {
        int conflict = 0;
        int update_section = 0;
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO t_no_conflicts;
*/

#line 87 "enroll.sqc"

        
/*
EXEC SQL select distinct section into : temp from section where term = (select distinct term from enrollment where grade is null) and pnum = : id and day = : day and time = : time;
*/

{
#line 88 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 88 "enroll.sqc"
  sqlaaloc(2,3,16,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 88 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 88 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 88 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 88 "enroll.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 11;
#line 88 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)day;
#line 88 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 88 "enroll.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 9;
#line 88 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)time;
#line 88 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 88 "enroll.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 88 "enroll.sqc"
  sqlaaloc(3,1,17,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 88 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 88 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 88 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 88 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 88 "enroll.sqc"
  sqlacall((unsigned short)24,8,2,3,0L);
#line 88 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 88 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto t_no_conflicts;
  }
#line 88 "enroll.sqc"
  sqlastop(0L);
}

#line 88 "enroll.sqc"


        conflict = 1;
        printf("Warning: time conflict\n");

    t_no_conflicts:

        
/*
EXEC SQL WHENEVER NOT FOUND GO TO t_end_set_update;
*/

#line 95 "enroll.sqc"

        
/*
EXEC SQL select distinct pnum into : temp from section where cnum = : cnum and term = (select distinct term from enrollment where grade is null) and section = : section;
*/

{
#line 96 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 96 "enroll.sqc"
  sqlaaloc(2,2,18,0L);
    {
      struct sqla_setdata_list sql_setdlist[2];
#line 96 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 96 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 96 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 96 "enroll.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 96 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)&section;
#line 96 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 96 "enroll.sqc"
      sqlasetdata(2,0,2,sql_setdlist,0L,0L);
    }
#line 96 "enroll.sqc"
  sqlaaloc(3,1,19,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 96 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 96 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 96 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 96 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 96 "enroll.sqc"
  sqlacall((unsigned short)24,9,2,3,0L);
#line 96 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 96 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto t_end_set_update;
  }
#line 96 "enroll.sqc"
  sqlastop(0L);
}

#line 96 "enroll.sqc"


        update_section = 1;

    t_end_set_update:

        if (update_section == 0)
        {
            
/*
EXEC SQL insert into section(cnum, term, section, pnum, day, time) values(
                : cnum, (select distinct term from enrollment where grade is null),
                : section,
                : id,
                : day,
                : time);
*/

{
#line 109 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 109 "enroll.sqc"
  sqlaaloc(2,5,20,0L);
    {
      struct sqla_setdata_list sql_setdlist[5];
#line 109 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 109 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 109 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 109 "enroll.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 109 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)&section;
#line 109 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 109 "enroll.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 109 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)&id;
#line 109 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 109 "enroll.sqc"
      sql_setdlist[3].sqltype = 460; sql_setdlist[3].sqllen = 11;
#line 109 "enroll.sqc"
      sql_setdlist[3].sqldata = (void*)day;
#line 109 "enroll.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 109 "enroll.sqc"
      sql_setdlist[4].sqltype = 460; sql_setdlist[4].sqllen = 9;
#line 109 "enroll.sqc"
      sql_setdlist[4].sqldata = (void*)time;
#line 109 "enroll.sqc"
      sql_setdlist[4].sqlind = 0L;
#line 109 "enroll.sqc"
      sqlasetdata(2,0,5,sql_setdlist,0L,0L);
    }
#line 109 "enroll.sqc"
  sqlacall((unsigned short)24,10,2,0,0L);
#line 109 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 109 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto t_end_set_update;
  }
#line 109 "enroll.sqc"
  sqlastop(0L);
}

#line 109 "enroll.sqc"


            if (conflict == 0)
            {
                printf("OK\n");
            }
        }
        else
        {
            
/*
EXEC SQL update section set pnum = : id where cnum = : cnum and term = (select distinct term from enrollment where grade is null) and section = : section;
*/

{
#line 118 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 118 "enroll.sqc"
  sqlaaloc(2,3,21,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 118 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 118 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 118 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 118 "enroll.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 6;
#line 118 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)cnum;
#line 118 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 118 "enroll.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 118 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)&section;
#line 118 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 118 "enroll.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 118 "enroll.sqc"
  sqlacall((unsigned short)24,11,2,0,0L);
#line 118 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 118 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto t_end_set_update;
  }
#line 118 "enroll.sqc"
  sqlastop(0L);
}

#line 118 "enroll.sqc"


            if (conflict == 0)
            {
                printf("OK\n");
            }
        }
    }
    else if (activity_type == 'O')
    {
        int conflict = 0;
        int update_oh = 0;
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO oh_no_conflicts;
*/

#line 130 "enroll.sqc"

        
/*
EXEC SQL select distinct pnum into : temp from officehour where pnum = : id and day = : day and time = : time;
*/

{
#line 131 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 131 "enroll.sqc"
  sqlaaloc(2,3,22,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 131 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 131 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 131 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 131 "enroll.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 11;
#line 131 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)day;
#line 131 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 131 "enroll.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 9;
#line 131 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)time;
#line 131 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 131 "enroll.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 131 "enroll.sqc"
  sqlaaloc(3,1,23,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 131 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 131 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 131 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 131 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 131 "enroll.sqc"
  sqlacall((unsigned short)24,12,2,3,0L);
#line 131 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 131 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto oh_no_conflicts;
  }
#line 131 "enroll.sqc"
  sqlastop(0L);
}

#line 131 "enroll.sqc"


        conflict = 1;
        printf("Warning: time conflict\n");

    oh_no_conflicts:
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO o_end_set_update;
*/

#line 137 "enroll.sqc"

        
/*
EXEC SQL select distinct pnum into : temp from officehour where cnum = : cnum and term = (select distinct term from enrollment where grade is null) and pnum = : id and day = : day and time = : time;
*/

{
#line 138 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 138 "enroll.sqc"
  sqlaaloc(2,4,24,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 138 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 138 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 138 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 138 "enroll.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 138 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)&id;
#line 138 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 138 "enroll.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 11;
#line 138 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)day;
#line 138 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 138 "enroll.sqc"
      sql_setdlist[3].sqltype = 460; sql_setdlist[3].sqllen = 9;
#line 138 "enroll.sqc"
      sql_setdlist[3].sqldata = (void*)time;
#line 138 "enroll.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 138 "enroll.sqc"
      sqlasetdata(2,0,4,sql_setdlist,0L,0L);
    }
#line 138 "enroll.sqc"
  sqlaaloc(3,1,25,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 138 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 138 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 138 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 138 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 138 "enroll.sqc"
  sqlacall((unsigned short)24,13,2,3,0L);
#line 138 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 138 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto o_end_set_update;
  }
#line 138 "enroll.sqc"
  sqlastop(0L);
}

#line 138 "enroll.sqc"


        update_oh = 1;

    o_end_set_update:

        if (update_oh == 0)
        {
            printf("insert\n");
            
/*
EXEC SQL insert into officehour(cnum, term, pnum, day, time) values(
                : cnum, (select distinct term from enrollment where grade is null),
                : id,
                : day,
                : time);
*/

{
#line 151 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 151 "enroll.sqc"
  sqlaaloc(2,4,26,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 151 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 151 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)cnum;
#line 151 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 151 "enroll.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 151 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)&id;
#line 151 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 151 "enroll.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 11;
#line 151 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)day;
#line 151 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 151 "enroll.sqc"
      sql_setdlist[3].sqltype = 460; sql_setdlist[3].sqllen = 9;
#line 151 "enroll.sqc"
      sql_setdlist[3].sqldata = (void*)time;
#line 151 "enroll.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 151 "enroll.sqc"
      sqlasetdata(2,0,4,sql_setdlist,0L,0L);
    }
#line 151 "enroll.sqc"
  sqlacall((unsigned short)24,14,2,0,0L);
#line 151 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 151 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto o_end_set_update;
  }
#line 151 "enroll.sqc"
  sqlastop(0L);
}

#line 151 "enroll.sqc"

        }

        if (conflict == 0)
        {
            printf("OK\n");
        }
    }
    else
    {
        int conflict = 0;
        int update_e = 0;
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO e_no_conflicts;
*/

#line 163 "enroll.sqc"

        
/*
EXEC SQL select distinct snum into : temp from enrollment as e inner join section as s on s.cnum = e.cnum and s.term = e.term and s.section = e.section where e.snum = : id and e.term = (select distinct term from enrollment where grade is null) and s.day = : day and s.time = : time;
*/

{
#line 164 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 164 "enroll.sqc"
  sqlaaloc(2,3,27,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 164 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 164 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 164 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 164 "enroll.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 11;
#line 164 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)day;
#line 164 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 164 "enroll.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 9;
#line 164 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)time;
#line 164 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 164 "enroll.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 164 "enroll.sqc"
  sqlaaloc(3,1,28,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 164 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 164 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&temp;
#line 164 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 164 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 164 "enroll.sqc"
  sqlacall((unsigned short)24,15,2,3,0L);
#line 164 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 164 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto e_no_conflicts;
  }
#line 164 "enroll.sqc"
  sqlastop(0L);
}

#line 164 "enroll.sqc"


        conflict = 1;
        printf("Warning: time conflict\n");

    e_no_conflicts:
        
/*
EXEC SQL WHENEVER NOT FOUND GO TO c_end_set_update;
*/

#line 170 "enroll.sqc"

        
/*
EXEC SQL select term into : temp_str from enrollment where snum = : id and cnum = : cnum and term = (select distinct term from enrollment where grade is null) and section = : section;
*/

{
#line 171 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 171 "enroll.sqc"
  sqlaaloc(2,3,29,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 171 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 171 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 171 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 171 "enroll.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 6;
#line 171 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)cnum;
#line 171 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 171 "enroll.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 171 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)&section;
#line 171 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 171 "enroll.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 171 "enroll.sqc"
  sqlaaloc(3,1,30,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 171 "enroll.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 20;
#line 171 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)temp_str;
#line 171 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 171 "enroll.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 171 "enroll.sqc"
  sqlacall((unsigned short)24,16,2,3,0L);
#line 171 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 171 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto c_end_set_update;
  }
#line 171 "enroll.sqc"
  sqlastop(0L);
}

#line 171 "enroll.sqc"


        update_e = 1;

    c_end_set_update:

        if (update_e == 0)
        {
            
/*
EXEC SQL insert into enrollment(snum, cnum, term, section) values(
                : id,
                : cnum, (select distinct term from enrollment where grade is null),
                : section);
*/

{
#line 182 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 182 "enroll.sqc"
  sqlaaloc(2,3,31,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 182 "enroll.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 182 "enroll.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 182 "enroll.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 182 "enroll.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 6;
#line 182 "enroll.sqc"
      sql_setdlist[1].sqldata = (void*)cnum;
#line 182 "enroll.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 182 "enroll.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 182 "enroll.sqc"
      sql_setdlist[2].sqldata = (void*)&section;
#line 182 "enroll.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 182 "enroll.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 182 "enroll.sqc"
  sqlacall((unsigned short)24,17,2,0,0L);
#line 182 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 182 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto c_end_set_update;
  }
#line 182 "enroll.sqc"
  sqlastop(0L);
}

#line 182 "enroll.sqc"

        }

        if (conflict == 0)
        {
            printf("OK\n");
        }
    }

    
/*
EXEC SQL COMMIT;
*/

{
#line 191 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 191 "enroll.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 191 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 191 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto c_end_set_update;
  }
#line 191 "enroll.sqc"
  sqlastop(0L);
}

#line 191 "enroll.sqc"

    
/*
EXEC SQL CONNECT reset;
*/

{
#line 192 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 192 "enroll.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 192 "enroll.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 192 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto c_end_set_update;
  }
#line 192 "enroll.sqc"
  sqlastop(0L);
}

#line 192 "enroll.sqc"

    exit(0);

error_no_student_professor:
    printf("Error: unknown professor/student id\n");
    goto error;
error_assign_lec_to_non_prof:
    printf("Error: assigning lecture to a non-professor\n");
    goto error;
error_assign_oh_to_non_prof:
    printf("Error: assigning office hours to a non-professor\n");
    goto error;
error_oh_for_unknown_class:
    printf("Error: office hours for an unknown class\n");
    goto error;
error_enrolling_non_student:
    printf("Error: enrolling a non-student in a class\n");
    goto error;
error_class_doesnt_exist:
    printf("Error: unknown class\n");
    goto error;
error:
    check_error("My error", &sqlca);
    
/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 215 "enroll.sqc"


    
/*
EXEC SQL ROLLBACK;
*/

{
#line 217 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 217 "enroll.sqc"
  sqlacall((unsigned short)28,0,0,0,0L);
#line 217 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto c_end_set_update;
  }
#line 217 "enroll.sqc"
  sqlastop(0L);
}

#line 217 "enroll.sqc"

    
/*
EXEC SQL CONNECT reset;
*/

{
#line 218 "enroll.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 218 "enroll.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 218 "enroll.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto c_end_set_update;
  }
#line 218 "enroll.sqc"
  sqlastop(0L);
}

#line 218 "enroll.sqc"

    exit(1);
}

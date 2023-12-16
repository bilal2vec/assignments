static char sqla_program_id[292] = 
{
 '\xac','\x0','\x41','\x45','\x41','\x56','\x41','\x49','\x64','\x42','\x32','\x4d','\x4d','\x5a','\x47','\x6d','\x30','\x31','\x31','\x31',
 '\x31','\x20','\x32','\x20','\x20','\x20','\x20','\x20','\x20','\x20','\x20','\x20','\x8','\x0','\x42','\x35','\x34','\x4b','\x48','\x41',
 '\x4e','\x20','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
 '\x0','\x0','\x8','\x0','\x53','\x43','\x48','\x45','\x44','\x55','\x4c','\x45','\x0','\x0','\x0','\x0','\x0','\x0','\x0','\x0',
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


#line 1 "schedule.sqc"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"


/*
EXEC SQL INCLUDE SQLCA;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "schedule.sqc"


int main(int argc, char *argv[])
{
    
/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 9 "schedule.sqc"

    char db[6] = "cs348";

    sqlint32 id;

    char name[31];
    sqlint32 is_student = 0;
    sqlint32 is_professor = 0;

    char current_term[6];

    char day[11];
    char time[9];
    char cnum[6];
    sqlint32 section;
    short  section_indicator;
    char activity_type[2];
    sqlint32 n_duplicates;

    
/*
EXEC SQL END DECLARE SECTION;
*/

#line 28 "schedule.sqc"


    id = atoi(argv[1]);

    
/*
EXEC SQL WHENEVER SQLERROR GO TO error;
*/

#line 32 "schedule.sqc"

    
/*
EXEC SQL CONNECT TO : db;
*/

{
#line 33 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 33 "schedule.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 33 "schedule.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 33 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)db;
#line 33 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 33 "schedule.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 33 "schedule.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 33 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 33 "schedule.sqc"
  sqlastop(0L);
}

#line 33 "schedule.sqc"


    
/*
EXEC SQL WHENEVER NOT FOUND GO TO get_professor;
*/

#line 35 "schedule.sqc"

    
/*
EXEC SQL SELECT sname INTO : name FROM student WHERE snum = : id;
*/

{
#line 36 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 36 "schedule.sqc"
  sqlaaloc(2,1,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 36 "schedule.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 36 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 36 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 36 "schedule.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 36 "schedule.sqc"
  sqlaaloc(3,1,3,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 36 "schedule.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 31;
#line 36 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)name;
#line 36 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 36 "schedule.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 36 "schedule.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 36 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 36 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto get_professor;
  }
#line 36 "schedule.sqc"
  sqlastop(0L);
}

#line 36 "schedule.sqc"

    is_student = 1;

get_professor:
    
/*
EXEC SQL WHENEVER NOT FOUND GO TO get_professor_end;
*/

#line 40 "schedule.sqc"

    
/*
EXEC SQL SELECT pname INTO : name FROM professor WHERE pnum = : id;
*/

{
#line 41 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 41 "schedule.sqc"
  sqlaaloc(2,1,4,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 41 "schedule.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 41 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)&id;
#line 41 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 41 "schedule.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 41 "schedule.sqc"
  sqlaaloc(3,1,5,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 41 "schedule.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 31;
#line 41 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)name;
#line 41 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 41 "schedule.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 41 "schedule.sqc"
  sqlacall((unsigned short)24,2,2,3,0L);
#line 41 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 41 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto get_professor_end;
  }
#line 41 "schedule.sqc"
  sqlastop(0L);
}

#line 41 "schedule.sqc"

    is_professor = 1;
get_professor_end:

    if (!is_student && !is_professor)
    {
        printf("Error\n");
        exit(0);
    }
    if (is_student || is_professor)
    {
        printf("%s\n", name);
    }

    
/*
EXEC SQL select distinct term into : current_term from enrollment where grade is null;
*/

{
#line 55 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 55 "schedule.sqc"
  sqlaaloc(3,1,6,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 55 "schedule.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 55 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)current_term;
#line 55 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 55 "schedule.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 55 "schedule.sqc"
  sqlacall((unsigned short)24,3,0,3,0L);
#line 55 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 55 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto get_professor_end;
  }
#line 55 "schedule.sqc"
  sqlastop(0L);
}

#line 55 "schedule.sqc"


    printf("%s\n", current_term);

    
/*
EXEC SQL DECLARE schedule CURSOR FOR select activities.day, activities.time, activities.cnum, activities.section, activities.activity_type, conflicts.n_duplicates from((select s.term, s.pnum as id, s.day, s.time, s.cnum, s.section, 'A' as activity_type from section as s) union all(select o.term, o.pnum as id, o.day, o.time, o.cnum, null as SECTION, 'B' as activity_type from officehour as o) union all(select s.term, e.snum as id, s.day, s.time, s.cnum, s.section, 'C' as activity_type from section as s inner join enrollment as e on e.cnum = s.cnum and e.term = s.term and e.section = s.section)) as activities, (select activities.term, activities.id, count(*) as n_duplicates, activities.day, activities.time from((select s.term, s.pnum as id, s.day, s.time, s.cnum, s.section, 'A' as activity_type from section as s) union all(select o.term, o.pnum as id, o.day, o.time, o.cnum, null as SECTION, 'B' as activity_type from officehour as o) union all(select s.term, e.snum as id, s.day, s.time, s.cnum, s.section, 'C' as activity_type from section as s inner join enrollment as e on e.cnum = s.cnum and e.term = s.term and e.section = s.section)) as activities group by activities.term, activities.id, activities.day, activities.time) as conflicts where conflicts.term = activities.term and conflicts.id = activities.id and conflicts.day = activities.day and conflicts.time = activities.time and activities.term = : current_term and activities.id = : id order by activities.activity_type, (case activities.day when 'Monday' then 1 when 'Tuesday' then 2 when 'Wednesday' then 3 when 'Thursday' then 4 when 'Friday' then 5 when 'Saturday' then 6 when 'Sunday' then 7 end), activities.time;
*/

#line 59 "schedule.sqc"


    
/*
EXEC SQL OPEN schedule;
*/

{
#line 61 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 61 "schedule.sqc"
  sqlaaloc(2,2,7,0L);
    {
      struct sqla_setdata_list sql_setdlist[2];
#line 61 "schedule.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 61 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)current_term;
#line 61 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 61 "schedule.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 61 "schedule.sqc"
      sql_setdlist[1].sqldata = (void*)&id;
#line 61 "schedule.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 61 "schedule.sqc"
      sqlasetdata(2,0,2,sql_setdlist,0L,0L);
    }
#line 61 "schedule.sqc"
  sqlacall((unsigned short)26,4,2,0,0L);
#line 61 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 61 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto get_professor_end;
  }
#line 61 "schedule.sqc"
  sqlastop(0L);
}

#line 61 "schedule.sqc"

    
/*
EXEC SQL WHENEVER NOT FOUND GO TO end;
*/

#line 62 "schedule.sqc"


    for (;;)
    {
        
/*
EXEC SQL FETCH schedule INTO : day, : time, : cnum, : section indicator : section_indicator, : activity_type, : n_duplicates;
*/

{
#line 66 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 66 "schedule.sqc"
  sqlaaloc(3,6,8,0L);
    {
      struct sqla_setdata_list sql_setdlist[6];
#line 66 "schedule.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 11;
#line 66 "schedule.sqc"
      sql_setdlist[0].sqldata = (void*)day;
#line 66 "schedule.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 66 "schedule.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 9;
#line 66 "schedule.sqc"
      sql_setdlist[1].sqldata = (void*)time;
#line 66 "schedule.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 66 "schedule.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 6;
#line 66 "schedule.sqc"
      sql_setdlist[2].sqldata = (void*)cnum;
#line 66 "schedule.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 66 "schedule.sqc"
      sql_setdlist[3].sqltype = 497; sql_setdlist[3].sqllen = 4;
#line 66 "schedule.sqc"
      sql_setdlist[3].sqldata = (void*)&section;
#line 66 "schedule.sqc"
      sql_setdlist[3].sqlind = &section_indicator;
#line 66 "schedule.sqc"
      sql_setdlist[4].sqltype = 460; sql_setdlist[4].sqllen = 2;
#line 66 "schedule.sqc"
      sql_setdlist[4].sqldata = (void*)activity_type;
#line 66 "schedule.sqc"
      sql_setdlist[4].sqlind = 0L;
#line 66 "schedule.sqc"
      sql_setdlist[5].sqltype = 496; sql_setdlist[5].sqllen = 4;
#line 66 "schedule.sqc"
      sql_setdlist[5].sqldata = (void*)&n_duplicates;
#line 66 "schedule.sqc"
      sql_setdlist[5].sqlind = 0L;
#line 66 "schedule.sqc"
      sqlasetdata(3,0,6,sql_setdlist,0L,0L);
    }
#line 66 "schedule.sqc"
  sqlacall((unsigned short)25,4,0,3,0L);
#line 66 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 66 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto end;
  }
#line 66 "schedule.sqc"
  sqlastop(0L);
}

#line 66 "schedule.sqc"


        char section_val = '-';
        if (section_indicator >= 0)
        {
            section_val = section + '0';
        }

        if (activity_type[0] == 'A')
        {
            activity_type[0] = 'S';
        }
        else if (activity_type[0] == 'B')
        {
            activity_type[0] = 'O';
        }
        else if (activity_type[0] == 'C')
        {
            activity_type[0] = 'C';
        }

        time[2] = '.';
        time[5] = '.';

        printf("%s %s %s %c %s", day, time, cnum, section_val, activity_type);

        if (n_duplicates > 1)
        {
            printf("*\n");
        }
        else
        {
            printf("\n");
        }
    }

end:
    
/*
EXEC SQL COMMIT;
*/

{
#line 103 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 103 "schedule.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 103 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 103 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto end;
  }
#line 103 "schedule.sqc"
  sqlastop(0L);
}

#line 103 "schedule.sqc"

    
/*
EXEC SQL CONNECT reset;
*/

{
#line 104 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 104 "schedule.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 104 "schedule.sqc"
  if (sqlca.sqlcode < 0)
  {
    sqlastop(0L);
    goto error;
  }
#line 104 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto end;
  }
#line 104 "schedule.sqc"
  sqlastop(0L);
}

#line 104 "schedule.sqc"

    exit(0);

error:
    check_error("My error", &sqlca);
    
/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 109 "schedule.sqc"


    
/*
EXEC SQL ROLLBACK;
*/

{
#line 111 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 111 "schedule.sqc"
  sqlacall((unsigned short)28,0,0,0,0L);
#line 111 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto end;
  }
#line 111 "schedule.sqc"
  sqlastop(0L);
}

#line 111 "schedule.sqc"

    
/*
EXEC SQL CONNECT reset;
*/

{
#line 112 "schedule.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 112 "schedule.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 112 "schedule.sqc"
  if (sqlca.sqlcode == 100)
  {
    sqlastop(0L);
    goto end;
  }
#line 112 "schedule.sqc"
  sqlastop(0L);
}

#line 112 "schedule.sqc"

    exit(1);
}
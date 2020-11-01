# Simple Calendar Application

## 1. Description

This is a project for a basics course in C programming.  
  
This application works as a calendar. It handles entries that have a description and a time (consisting of month, day and hour - this application knows only one year at a time). The user can add or remove entries, save all of the calendar's entries to a file, and respectively, download entries from a formatted file. In addition to that, the user can view their calendar that is sorted by the date.  
  
Every task is expected to last precisely one hour, and simultaneus tasks cannot be added to the calendar. Therefore, every entry's date (month, day, and hour) must be unique. As command parts are separated by a space, the length of an entry description is expected to be only one word. The maximum length of a command is 80 characters.  
  
When a calendar is loaded from a file, the application replaces its possible previous entries with the new ones from the file. If the loading fails, the entries will remain.  
  
## 2. User interface
  
The interface is text-based. The application recognizes the following commands.  

### 2.1 Commands

| TASK                          | COMMAND                       |
| :---                          | :---                          |
| Add a new entry               | A description month day hour  |
| Remove an existing entry      | D month day hour              |
| Save calendar to a file       | W filename                    |
| Download calendar from a file | O filename                    |
| Print calendar                | L                             |
| Quit                          | Q                             |

## 3. Use case example

The example below showcases how this calendar can be used. The user has a file called saved.txt to which the calendar entries can be saved.  
  
*Italicized text* represents user input.  
  
Welcome!  
Accepted commands:  
ADD A NEW ENTRY:                A description month day hour  
REMOVE AN EXISTING ENTRY:       D month day hour  
SAVE CALENDAR TO A FILE:        W filename  
DOWNLOAD CALENDAR FROM A FILE:  O filename  
PRINT CALENDAR:                 L  
QUIT:                           Q  
Please give time and date as numbers.  
*A math-lecture 10 10 10*  
Entry added successfully.  
*A hairdresser 11 11 11*  
Entry added successfully.  
*A meeting 10 10 10*  
Given date is already taken!  
*D 10 10 10*  
Entry removed successfully.  
*A meeting 10 10 10*  
Entry added successfully.  
*L*  
meeting 10.10. at 10  
hairdresser 11.11. at 11  
*W saved.txt*  
Calendar saved!  
*D 10 10 10*  
Entry removed successfully.  
*D 10 10 10*  
Given date is already free.  
*D 11 11 11*  
Entry removed successfully.  
*L*  
Your calendar is empty.  
*O saved.txt*  
1... Entry added successfully.  
2... Entry added successfully.  
2 out of 2 entries added to your calendar.  
*L*  
meeting 10.10. at 10  
hairdresser 11.11. at 11  
*Q*  
Thank you for using calendar.  

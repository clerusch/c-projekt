#include <stdlib.h>

#include "scheduler.h"

struct Process* rr(struct Process* head, struct Process* current) {


	//am Anfang returnen wir immer den ersten Prozess
	if (current == NULL)
		return head->next;
	//wenn keine Prozesse mehr da sind brechen wir ab
	else if (head->next == head)
		return NULL;
	//wir returnen immer einfach den nächsten Prozess
	else if (current->next != head)
		return current->next;
	//und wenn wir beim head angekommen sind, fangen wir wieder von vorne an
	else
		return head->next;

}

struct Process* fcfs(struct Process* head, struct Process* current) {

	//am Anfang wieder der erste Prozess
	if (current == NULL)
		return head->next;
	//wenn keine Prozesse mehr da sind, Abbruch
	else if (head->next == head)
		return NULL;
	//wenn der aktuelle Prozess noch zu arbeiten hat, dann lassen wir ihn arbeiten
	else if (current->cycles_todo > 0)
		return current;
	//und wenn er fertig ist, springen wir zum nächsten Prozess
	else //if (current->next != head)
		return current->next;
//	else 
//		return head->next;

}

struct Process* spn(struct Process* head, struct Process* current) {
	return NULL;
}


struct Process* srt(struct Process* head, struct Process* current) {

	struct Process* cursor = head->next;
	current = head->next;

	if (head->next != head)
	{
		//wir gehen die Liste jedesmal komplett durch und suchen das Element mit
		//dem kleinsten Wert von cycles_todo und geben dieses zurück
		while (cursor != head)
		{
			if (current->cycles_todo > cursor->cycles_todo) 
			{
				current = cursor;
			}

			cursor = cursor->next;
		}
		
		return current;
	}
	else 
		return NULL;
}

struct Process* hrrnPreemptive(struct Process* head, struct Process* current) {
	return NULL;
}

struct Process* hrrnNonPreemptive(struct Process* head, struct Process* current) {
	return NULL;
}


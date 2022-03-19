/*
Musuroi Daniel-Nicusor
Anul II
Grupa:313CBa
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

File *alloc_File(char *name, Dir *parent) {

	File *new_file = (File *)malloc(sizeof(File));
	if (!new_file) {
		exit(5);
	}

	new_file->name = (char *)malloc((sizeof(name) + 1) * sizeof(char));
	if (!(new_file->name)) {
		exit(5);
	}

	strcpy(new_file->name, name);

	new_file->parent = parent;
	new_file->next = NULL;
	return new_file;

}

void destroy_File(File **file) {

	free((*file)->name);
	free(*file);
	*file = NULL;
}

Dir *alloc_Dir(char *name, Dir *parent) {
	
	Dir *new_dir = (Dir *)malloc(sizeof(Dir));
	if (!new_dir) {
		exit(5);
	}

	new_dir->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
	if (!(new_dir->name)) {
		exit(5);
	}

	strcpy(new_dir->name, name);
	new_dir->parent = parent;
	new_dir->next = NULL;
	new_dir->head_children_dirs = NULL;
	new_dir->head_children_files = NULL;
	return new_dir;

}

void destroy_Dir(Dir **director) {

	free((*director)->name);
	free(*director);
	*director = NULL;
}

void destroy_allfromDir(Dir **director) {
	//if list not empty:
	if ((*director)->head_children_files) {
		File *ant_file = (*director)->head_children_files;
		File *p_file = ant_file->next;
		for (; p_file != NULL; ant_file = p_file, p_file = p_file->next) {
			destroy_File(&ant_file);
		}
		//destroy the last file from the list:
		destroy_File(&ant_file);
	}

	//if list not empty:
	if((*director)->head_children_dirs) {
		Dir *ant_dir = (*director)->head_children_dirs;
		Dir *p_dir = ant_dir->next;
		for (; p_dir != NULL; ant_dir = p_dir, p_dir = p_dir->next) {
			destroy_allfromDir(&ant_dir);
		}
		//destroy the last dir from the list:
		destroy_allfromDir(&ant_dir);
	}

	destroy_Dir(director);
}

void touch(Dir* parent, char* name) {
	Dir *p_dir = parent->head_children_dirs;
	for (; p_dir != NULL; p_dir = p_dir->next) {
		if (strcmp(p_dir->name, name) == 0) {
			printf("File already exists\n");
			return;
		}
	}

	if (parent->head_children_files == NULL) {
		File *new_file = alloc_File(name, parent);
		parent->head_children_files = new_file;
		return;
	}

	int already_exists = 0;
	File *p_file = parent->head_children_files;
	for (; p_file->next != NULL && !already_exists; p_file = p_file->next) {
		if (strcmp(p_file->name, name) == 0) {
			already_exists = 1;
		}
	}


	//check for the last element of the list
	if (strcmp(p_file->name, name) == 0) {
		already_exists = 1;
	}

	if (already_exists) {
		printf("File already exists\n");
	} else {
		File *new_file = alloc_File(name, parent);
		p_file->next = new_file;
	}
}

void mkdir(Dir* parent, char* name) {
	
	File *p_file = parent->head_children_files;
	for (; p_file != NULL; p_file = p_file->next) {
		if (strcmp(p_file->name, name) == 0) {
			printf("Directory already exists\n");
			return;
		}
	}

	if (parent->head_children_dirs == NULL) {
		Dir *new_dir = alloc_Dir(name, parent);
		parent->head_children_dirs = new_dir;
		return;
	}

	int already_exists = 0;

	Dir *p_dir = parent->head_children_dirs;
	for (; p_dir->next != NULL && !already_exists; p_dir = p_dir->next) {
		if (strcmp(p_dir->name, name) == 0) {
			already_exists = 1;
		}
	}
	//check for the last element of the list
	if (strcmp(p_dir->name, name) == 0) {
		already_exists = 1;
	}

	if (already_exists) {
		printf("Directory already exists\n");
	} else {
		Dir *new_dir = alloc_Dir(name, parent);
		p_dir->next = new_dir;
	} 
}

void ls(Dir* parent) {
	Dir *p_dir = parent->head_children_dirs;
	for (; p_dir != NULL; p_dir = p_dir->next) {
		printf("%s\n", p_dir->name);
	}

	File *p_file = parent->head_children_files;
	for (; p_file != NULL; p_file = p_file->next) {
		printf("%s\n", p_file->name);
	}

}

void rm(Dir* parent, char* name) {
	//empty list:
	if (parent->head_children_files == NULL) {
		printf("Could not find the file\n");
		return;
	}

	File *ant = parent->head_children_files;
	File *p = ant->next;

	//if i have to remove the first file:
	if (strcmp(ant->name, name) == 0) {
		//remove the first file from the list:
		parent->head_children_files = p;
		destroy_File(&ant);
		return;
	}

	for (; p != NULL; ant = p, p = p->next) {
		if (strcmp(p->name, name) == 0) {
			//remove the file:
			ant->next = p->next;
			destroy_File(&p);
			return;
		}
	}

	//reach the end of the list:
	printf("Could not find the file\n");
}

void rmdir(Dir* parent, char* name) {
	//empty list:
	if (parent->head_children_dirs == NULL) {
		printf("Could not find the dir\n");
		return;
	}

	Dir *ant = parent->head_children_dirs;
	Dir *p = ant->next;

	//if i have to remove the first dir:
	if (strcmp(ant->name, name) == 0) {
		//remove the first dir from the list:
		parent->head_children_dirs = p;
		destroy_allfromDir(&ant);
		return;
	}

	for (; p != NULL; ant = p, p = p->next) {
		if (strcmp(p->name, name) == 0) {
			//remove the dir:
			ant->next = p->next;
			destroy_allfromDir(&p);
			return;
		}
	}

	//reach the end of the list:
	printf("Could not find the dir\n");
}

void cd(Dir** target, char *name) {
	if (strcmp(name, "..") == 0) {
		if ((*target)->parent) {
			*target = (*target)->parent;
		}
		return;
	}

	Dir *p = (*target)->head_children_dirs;
	for (; p != NULL; p = p->next) {
		if (strcmp(p->name, name) == 0) {
			*target = p;
			return;
		}
	}

	printf("No directories found!\n");
}

//destroy everything from target and destroy target

void tree(Dir* target, int level) {
	
	Dir *p_dir = target->head_children_dirs;
	for (; p_dir != NULL; p_dir = p_dir->next) {

		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		printf("%s\n", p_dir->name);

		tree(p_dir, level + 1);
	}

	File *p_file = target->head_children_files;
	for (; p_file != NULL; p_file = p_file->next) {

		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		printf("%s\n", p_file->name);
	}
}

char *aux_pwd(Dir *target, int len) {
	//add 1 for "/"
	len = len + strlen(target->name) + 1;
	char *rez;
	if (strcmp(target->name, "home") == 0) {
		rez = (char *)malloc((len + 1) * sizeof(char));
		if (!rez) {
			exit(5);
		}
		rez[0] = '\0';

	} else {
		rez = aux_pwd(target->parent, len);
	}

	strcat(rez, "/");
	strcat(rez, target->name);

	return rez;
}

char *pwd(Dir* target) {
	return aux_pwd(target, 0);
}

void stop(Dir* target) {
	destroy_allfromDir(&target);
}

int already_exists(Dir *parent, char *name) {

	File *p_file = parent->head_children_files;
	for (; p_file != NULL; p_file = p_file->next) {
		if (strcmp(p_file->name, name) == 0) {
			return 1;
		}
	}

	Dir *p_dir = parent->head_children_dirs;
	for (; p_dir != NULL; p_dir = p_dir->next) {
		if (strcmp(p_dir->name, name) == 0) {
			return 1;
		}
	}

	return 0;
}

void mv(Dir* parent, char *oldname, char *newname) {
	/* 
	found = 0 -> file/director with name oldname not found
	found = 1 -> a file with name oldname was found
	found = 2 -> a director with name oldname was found
	*/
	
	int found = 0;

	File *ant_file = NULL;
	File *p_file = parent->head_children_files;
	for (; p_file != NULL; ant_file = p_file, p_file = p_file->next) {
		if (strcmp(p_file->name, oldname) == 0) {
			found = 1;
			break;
		}
	}

	Dir *ant_dir = NULL;
	Dir *p_dir = parent->head_children_dirs;
	for (; p_dir && !found; ant_dir = p_dir, p_dir = p_dir->next) {
		if (strcmp(p_dir->name, oldname) == 0) {
			found = 2;
			break;
		}
	}

	if (!found) {
		printf("File/Director not found\n");
		return;
	}

	if (already_exists(parent, newname)) {
		printf("File/Director already exists\n");
		return;
	}

	//if a file with oldname was found
	if (found == 1) {
		strcpy(p_file->name, newname);
		//if the file was the first in the list:
		if (ant_file == NULL) {
			//if the list has more than one file:
			if (p_file->next) {
				parent->head_children_files = p_file->next;
			}
			ant_file = parent->head_children_files;
		} else {
			ant_file->next = p_file->next;
		}

		for (; ant_file->next != NULL; ant_file = ant_file->next);
		ant_file->next = p_file;
		p_file->next = NULL;
		return;
	}

	//if a dir with oldname was found
	if (found == 2) {
		strcpy(p_dir->name, newname);
		//if the dir was the first in the list:
		if (ant_dir == NULL) {
			//if the list has more than one dir:
			if (p_dir->next) {
				parent->head_children_dirs = p_dir->next;
			}
			ant_dir = parent->head_children_dirs;
		} else {
			ant_dir->next = p_dir->next;
		}

		for (; ant_dir->next != NULL; ant_dir = ant_dir->next);
		ant_dir->next = p_dir;
		p_dir->next = NULL;
		return;
	}
}

int main () {
	char *line = (char *)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	if (!line) {
		exit(5);
	}

	Dir *home = alloc_Dir("home", NULL);
	Dir *curr_dir = home;
	char *command;

	do
	{

		fgets(line, MAX_INPUT_LINE_SIZE + 1, stdin);
		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}

		command = strtok(line, " ");

		if (strcmp(command, "touch") == 0) {
			char *name = strtok(NULL, " ");
			touch(curr_dir, name);
		}

		if (strcmp(command, "mkdir") == 0) {
			char *name = strtok(NULL, " ");
			mkdir(curr_dir, name);
		}

		if (strcmp(command, "ls") == 0) {
			ls(curr_dir);
		}

		if (strcmp(command, "rm") == 0) {
			char *name = strtok(NULL, " ");
			rm(curr_dir, name);
		}

		if (strcmp(command, "rmdir") == 0) {
			char *name = strtok(NULL, " ");
			rmdir(curr_dir, name);
		}

		if (strcmp(command, "cd") == 0) {
			char *name = strtok(NULL, " ");
			cd(&curr_dir, name);
		}

		if (strcmp(command, "tree") == 0) {
			tree(curr_dir, 0);
		}

		if (strcmp(command, "pwd") == 0) {
			char *pwd_string = pwd(curr_dir);
			printf("%s\n", pwd_string);
			free(pwd_string);
		}

		if (strcmp(command, "stop") == 0) {
			stop(home);
		}

		if (strcmp(command, "mv") == 0) {
			char *oldname = strtok(NULL, " ");
			char *newname = strtok(NULL, " ");
			mv(curr_dir, oldname, newname);
		}

	} while (strcmp(command, "stop"));

	free(line);

	return 0;
}

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_SIZE 30

typedef enum { up, down, left, right } direction;

// A node of a linked list
typedef struct {
  uint x;
  uint y;
  direction direction;
  struct BodyPart *next;
} BodyPart;

int main(int argc, char *argv[]) {
  if (argc > 1) {
    printf("This program shouldn't take any argument.\n");
    return 1;
  }

  BodyPart *head = malloc(sizeof(BodyPart));
  head->x = 1;
  head->y = 1;
  head->direction = right;
  head->next = NULL;

  (void)setlocale(LC_ALL, "");  // Add support to UTF-8 characters
  (void)initscr();              // Initialize Ncurses
  (void)curs_set(0);            // Hide the cursor
  (void)noecho();
  (void)cbreak();  // don't interrupt for user input
  timeout(100);    // Draw one frame each 100ms

  WINDOW *win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);

  while (true) {
    // Clear the screen
    (void)wclear(win);

    // Drawing
    (void)box(win, 0, 0);
    (void)mvwprintw(win, 0, 1, "Snake");

    (void)mvwprintw(win, head->y, head->x, "█");

    // Head movement
    if (up == head->direction) head->y--;
    if (down == head->direction) head->y++;
    if (left == head->direction) head->x--;
    if (right == head->direction) head->x++;

    // Refresh the screen
    (void)refresh();
    (void)wrefresh(win);

    // Input handling
    char input = getch();
    if ('q' == input) break;
    if ('w' == input && down != head->direction) head->direction = up;
    if ('s' == input && up != head->direction) head->direction = down;
    if ('a' == input && right != head->direction) head->direction = left;
    if ('d' == input && left != head->direction) head->direction = right;
  }

  // Free all the snake's body parts
  BodyPart *current_body_part = head;
  while (NULL != current_body_part) {
    BodyPart *temp = current_body_part;
    current_body_part = (BodyPart *)current_body_part->next;
    free(temp);
  }

  (void)endwin();

  return EXIT_SUCCESS;
}

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WINDOW_SIZE 30

// A node of a linked list
typedef struct {
  uint x;
  uint y;
  enum direction { up, down, left, right } direction;
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

  srand(time(0));
  uint apple_x = rand() % (WINDOW_SIZE - 2) + 1;
  uint apple_y = rand() % (WINDOW_SIZE - 2) + 1;

  (void)setlocale(LC_ALL, "");  // Add support to UTF-8 characters
  (void)initscr();              // Initialize Ncurses
  (void)curs_set(0);            // Hide the cursor
  (void)noecho();
  (void)cbreak();  // don't interrupt for user input
  timeout(100);    // Draw one frame each 100ms

  WINDOW *win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);

  bool game_over = false;
  while (!game_over) {
    // Clear the screen
    (void)wclear(win);

    // Drawing
    (void)box(win, 0, 0);                 // draw the borders of the window
    (void)mvwprintw(win, 0, 1, "Snake");  // Draw the title
    (void)mvwprintw(win, apple_y, apple_x, "*");  // Draw the apple
    BodyPart *current_body_part = head;
    do {
      (void)mvwprintw(win, current_body_part->y, current_body_part->x, "█");
      if (NULL != current_body_part->next)
        current_body_part = (BodyPart *)current_body_part->next;
    } while (NULL != current_body_part->next);

    /* When the snake eats an apple, in other words, when the head hits the
       apple Move the apple to a new random position and make the snake longer
     */
    if (apple_y == head->y && apple_x == head->x) {
      apple_x = rand() % (WINDOW_SIZE - 2) + 1;
      apple_y = rand() % (WINDOW_SIZE - 2) + 1;

      BodyPart *new = malloc(sizeof(BodyPart));
      new->x = head->x;
      new->y = head->y;
      new->direction = head->direction;

      if (up == head->direction) new->y++;
      if (down == head->direction) new->y--;
      if (left == head->direction) new->x++;
      if (right == head->direction) new->x--;

      new->next = (struct BodyPart *)head;
      head = new;
    }

    // Snake movement
    // Add a body part to the head
    BodyPart *new = malloc(sizeof(BodyPart));
    new->x = head->x;
    new->y = head->y;
    new->direction = head->direction;

    if (up == head->direction) new->y--;
    if (down == head->direction) new->y++;
    if (left == head->direction) new->x--;
    if (right == head->direction) new->x++;

    new->next = (struct BodyPart *)head;
    head = new;

    // Delete the last body part
    BodyPart *last_body_part = head;
    BodyPart *penultimate_body_part = head;
    while (NULL != last_body_part->next) {
      last_body_part = (BodyPart *)last_body_part->next;
      if (NULL != last_body_part->next)
        penultimate_body_part = (BodyPart *)last_body_part;
    }
    penultimate_body_part->next = NULL;
    free(last_body_part);

    // Refresh the screen
    (void)wrefresh(win);

    // Input handling
    char input = getch();
    if ('q' == input) break;
    if ('w' == input && down != head->direction) head->direction = up;
    if ('s' == input && up != head->direction) head->direction = down;
    if ('a' == input && right != head->direction) head->direction = left;
    if ('d' == input && left != head->direction) head->direction = right;

    // Game over checking
    if (head->x < 1 || head->x > WINDOW_SIZE - 2 || head->y < 1 ||
        head->y > WINDOW_SIZE - 2)
      game_over = true;
  }

  // Free all the snake's body parts
  BodyPart *current_body_part = head;
  while (NULL != current_body_part) {
    BodyPart *temp = current_body_part;
    current_body_part = (BodyPart *)current_body_part->next;
    free(temp);
  }

  if (game_over) {
    // Refresh the screen
    (void)werase(win);
    (void)wrefresh(win);
    (void)printf("Game over :(");
    (void)getchar();
  }

  (void)endwin();

  return EXIT_SUCCESS;
}

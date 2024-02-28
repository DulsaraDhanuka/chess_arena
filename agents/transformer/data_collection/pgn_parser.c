#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Result {
	WHITE_WON,
	BLACK_WON,
	DRAW,
	NA
};

struct ChessGame {
	enum Result result;
};

struct Tag {
	char *tag_name;
	char *tag_value;
};

struct Tag* parse_tag(int *pos, const char *pgn) {
	if (pgn[*pos] == '[') {
		struct Tag* tag = (struct Tag*)malloc(sizeof(struct Tag));
		while(pgn[*pos+1] == ' ') {
			*pos += 1;
		}
		
		int start_idx = *pos+1;
		while(pgn[*pos+1] != ' ' && pgn[*pos+1] != '"') {
			*pos += 1;
		}
		int end_idx = *pos+1;
		tag->tag_name = (char*)malloc(end_idx-start_idx+1);
		int k = 0;
		for (int j = start_idx; j < end_idx; j++) {
			tag->tag_name[k] = pgn[j];
			k++;
		}
		tag->tag_name[end_idx-start_idx] = '\0';

		while(pgn[*pos] != '"') {
			*pos += 1;
		}
		start_idx = *pos+1;
		while(pgn[*pos+1] != '"') {
			*pos += 1;
		}
		end_idx = *pos+1;

		tag->tag_value = (char*)malloc(end_idx-start_idx+1);
		k = 0;
		for (int j = start_idx; j < end_idx; j++) {
			tag->tag_value[k] = pgn[j];
			k++;
		}
		tag->tag_value[end_idx-start_idx] = '\0';

		while (pgn[*pos-1] != ']') {
			*pos += 1;
		}

		return tag;
	}

	return NULL;
}

void free_tag(struct Tag* tag) {
	free(tag->tag_name);
	free(tag->tag_value);
	free(tag);
}

int main(int argc, char *argv) {
	FILE *f;
	f = fopen("test1.pgn", "rb");

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *pgn = (char*)malloc(fsize+1);
	fread(pgn, fsize, 1, f);
	pgn[fsize] = 0;
	fclose(f);

	long pgn_size = fsize;
	//printf("%s", pgn);
	
	struct ChessGame *game = (struct ChessGame*)malloc(sizeof(struct ChessGame));
	game->result = NA;

	for (int i = 0; i < pgn_size; i++) {
		struct Tag* tag = parse_tag(&i, pgn);

		if (tag != NULL) {
			if (strcmp(tag->tag_name, "Result") == 0) {
				//printf("%s", tag->tag_value);
				if (strcmp(tag->tag_value, "0-1") == 0) {
					game->result = BLACK_WON;
				} else if (strcmp(tag->tag_value, "1-0") == 0) {
					game->result = WHITE_WON;
				} else if (strcmp(tag->tag_value, "1/2-1/2") == 0) {
					game->result = DRAW;
				} else {
					game->result = NA;
				}
			}
	
			free_tag(tag);
		}

		if (game->result != NA) {
			printf("%c", pgn[i]);
		}
	}

	return 0;
}

//______THREAT SEARCH________

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
int board_score = 0;
int turn = 0;
int* *tstranstable = 0;
int *tsinitTransptr = 0;
int *tstransptr = 0;
int tstransint = 0;
int* *transtable = 0;
int *initTransptr = 0;
int *transptr = 0;
int transint = 0;
int *initptr = 0;
int *initptr2 = 0;
int *initptr3 = 0;
int *currptr = 0;
int *currptr2 = 0;
int *currptr3 = 0;
int *maxptr = 0;
int *maxptr2 = 0;
int *maxptr3 = 0;
struct Node {
	int first;
	struct Node *rest;
};

int* best_line = 0;

struct pnNode { 
	int type;
	int move;
	int proof;
	int disproof;
	struct pfNode *(*chlidren);

};

struct Holder {
	int fEval;
	int *wLine;
	int* *wCosts;
};

struct Threat { 
	int type;
	int gain;
	int *cost;
	int depth;
	struct Threat *dependants;
	struct Threat *next;
	struct Threat *comb;
};

void *myTransCalloc() {
	int *myptr = transptr;
	transptr += 912;
	return myptr;
}

void *myTSTransCalloc() {
	int *myptr = tstransptr;
	tstransptr += 912;
	return myptr;
}

void addToTSTrans(int board[]) {
	//printf("ADDED!! %d\n", transint);
	if (tstransint < 100000) {
		int *newBoard = myTSTransCalloc();
		for (int i = 0; i < 225; i++) {
			newBoard[i] = board[i];
		}
		tstranstable[tstransint] = newBoard;
		tstransint += 1;
	} else {
		// printf("yoo!\n");
	}
}

void addToTrans(int board[], int score) {
	//printf("ADDED!! %d\n", transint);
	if (transint < 100000) {
		int *newBoard = myTransCalloc();
		for (int i = 0; i < 225; i++) {
			newBoard[i] = board[i];
		}
		newBoard[225] = score;
		transtable[transint] = newBoard;
		transint += 1;
	} else {
		// printf("yoo!\n");
	}
}

int inTable(int board[]) {
	int i = 0;
	int bob = 1;
	int score = 0;
	while (i < transint && bob) {
		if (transtable[i] != 0) {
			bob = 0;
			for (int j = 112; j >= 0; j++) {
				if (board[j] != transtable[i][j] || board[224 - j] != transtable[i][224 - j]) {
					bob = 1;
					break;
				}
				if (j == 224) {
					score = transtable[i][j+1];
				}
			}
		}
		i++;
	}
	return score;
}

int inTSTable(int board[]) {
	// printf("tstransint: %d\n", tstransint);
	int i = 0;
	int bob = 1;
	int score = 0;
	while (i < tstransint && bob) {
		if (tstranstable[i] != 0) {
			bob = 0;
			for (int j = 0; j < 225; j++) {
				if (board[j] != tstranstable[i][j]) {
					bob = 1;
					break;
				}
			}
		}
		i++;
	}
	return 1-bob;
}

void *myCalloc(int num, int memory) {
	int *myptr = currptr;
	if ((size_t) currptr <= (size_t) maxptr) {
		currptr += num*memory/4;
	}
	if((size_t) currptr > (size_t) maxptr) {
		myptr = currptr2;
		if ((size_t) currptr2 <= (size_t) maxptr2) {
			currptr2 += num*memory/4;
		}
		if ((size_t) currptr2 > (size_t) maxptr2) {
			myptr = currptr3;
			currptr3 += num*memory/4;
			assert((size_t) currptr3 <= (size_t) maxptr3);
		}
	}
	memset(myptr, 0, num*memory);
	return myptr;
}

struct Node *cons(int data, struct Node *lst) {
	struct Node *ptr = myCalloc(1 ,sizeof(struct Node));
	ptr->first = data;
	ptr->rest = lst;
	return ptr;
};

struct Node *ccons(int data, struct Node *lst) {
	struct Node *ptr = calloc(1 ,sizeof(struct Node));
	ptr->first = data;
	ptr->rest = lst;
	return ptr;
};

void freeHolder(struct Holder *res) {
	//printf("ur mom\n");
	for (int i = 0; i < 20; i++) {
		if((res->wCosts)[i] != NULL) {
			int *ptr = (res->wCosts)[i];
			//free(ptr);
		}
	}
	//free(res->wLine);
	//free(res->wCosts);
}

void freeLst(struct Node *lst) {
	if (lst != NULL) {
		freeLst(lst->rest);
	}
	free(lst);
}

int length(struct Threat *threaty) {
	int j = 0;
	for (struct Threat *yolo; yolo; yolo = yolo->next) {
		j++;
	}
	return j;
}

struct Holder *TSS(int board[], int player, int max);

int *create() {
	int *ptr = myCalloc(4, sizeof(int));
	for (int i = 0; i < 4; i++) {
		ptr[i] = -1;
	}
	return ptr;
}

int comp(int arr[], int n) {
	if (arr != 0) {
		for (int i = 0; i < 4; i++) {
			if (arr[i] != -1){
				if (arr[i] == n) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int comp2(int arr[], int arr2[]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (arr[i] == arr2[j] && arr[i] != -1) {
				return 0;
			}
		}
	}
	return 1;
}

void printBoard(int bd[]) {
	struct Node *white = 0;
	struct Node *black = 0;
	for (int j = 0; j < 15; j++) {
		for (int k = 0; k < 15; k++) {
			if (bd[j*15 + k] == 1) {
				white = cons(j*15 + k + 1 , white);
				printf("  X  ");
			} else if (bd[j*15 + k] == 2) {
				black = cons(j*15 + k + 1, black);
				printf("  O  ");
			} else if (j*15 + k < 10) {
				printf("  %d  ", j*15 + k);
			} else if (j*15 + k < 100) {
				printf("  %d ", j*15 + k);
			} else {
				printf(" %d ", j*15 + k);
			};
		};
		printf("\n\n");
	}; 
	for (; white; white = white->rest) {
		printf("cons(%d, ", white->first);
	}
	printf("\n");
	for (; black; black = black->rest) {
		printf("cons(%d, ", black->first);
	}
	printf("\n");
};

void printThreats(struct Threat *tlst, int d) {
	for (struct Threat *lst = tlst; lst; lst = lst->next) {
		// printf("Gain: %d, Cost: %d, Type: %d Depth: %d \n", lst->gain, *(lst->cost), lst->type, lst->depth);
		if(lst->dependants && d < 4) {
			printThreats(lst->dependants, d + 1);
		};
	};
};

void printThreatsC(struct Threat *tlst, int d) {
	for (struct Threat *lst = tlst; lst; lst = lst->next) {
		// printf("Gain: %d, Cost: %d, Type: %d Depth: %d \n", lst->gain, *(lst->cost), lst->type, d);
		if(lst->comb && d < 4) {
			printThreatsC(lst->comb, d + 1);
		};
		// printf("\n");
	};
};



void loadBoard(struct Node *white, struct Node *black, int board[]) {
	for (int i = 0; i < 225; i++) {
		board[i] = 0;
	};
	for (; white; white = white->rest) {
		board[white->first - 1] = 1;
	};
	for (; black; black = black->rest) {
		board[black->first - 1] = 2;
	};
};



int isValid(int sqrRow, int sqrCol, int p) {
	if (p >= 0 && p < 225 && p/15 - sqrCol < 5 && p/15 - sqrCol > -5 && (p % 15) - sqrRow > -5 && (p % 15) - sqrRow < 5) {
		return 1;
	};
	return 0;
};

struct Threat *isThreat(int move, int board[], int player, int max) {
	struct Threat *tt = NULL;
	int op;
	if (player == 1) {
		op = 2;
	} else {
		op = 1;
	};
	int con;
	int p;
	int sqrCol = move/15;
	int sqrRow = move % 15;
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			con = 1;
		} else if (i == 1) {
			con = 14;
		} else if (i == 2) {
			con = 15;
		} else {
			con = 16;
		};
		for (int l = 4; l > -1; l--) {
			int me = 0;
			int you = 0;
			int blank = -1;
			int blank2 = -1;
			int prev = 0;
			int *typee = calloc(5, sizeof(int));
			for (int g = 0; g < 5; g++) {
				typee[g] = 0;
			}
			for (int j = 0; j < 5; j++) {
				p = move + con*(j - l);
				if (isValid(sqrRow, sqrCol, p)) {
					// if (move == 80) {
					// 	printf("bd: %d mv: %d j: %d l: %d \n",  board[p], p, j, l);
					// };
					if (board[p] == player || p == move) {
						me++;
						typee[j] = 1;
						prev = 0;
					// } else if (board[p] == op && j == 4 && me == 3 && board[move + coe*con*(j - 1)] == 0) {
					// 	for (k = -1; k > -3; k--) {
					// 		int z = move + coe*con*k;
					// 		if (isValid(sqrRow, sqrCol, z)) {
					// 			if (board[z] == 0) {
					// 				blank++;
					// 			};
					// 		};
					// 	};
					// 	if (blank == 3) {
					// 		tt = myCalloc(1 ,sizeof(struct Threat));
					// 		tt->gain = move;
					// 		tt->cost = 
					// 	};
					} else if (board[p] == op) {
						break;
					} else if (blank > -1 && blank2 > -1 && board[p] == 0) {
						break;
					} else if (blank > -1 && board[p] == 0 && prev == 1) {
						break;
					} else if (blank > -1 && board[p] == 0) {
						blank2 = p;
					} else {
						prev = 1;
						blank = p;
					};
				};
			};
			if (me == 4 && blank > -1 && max < 5) {
				tt = myCalloc(1, sizeof(struct Threat));
				tt->gain = move;
				tt->type = 4;
				(tt->cost) = create();
				(tt->cost)[0] = blank;
				(tt->cost)[1] = -1;
				(tt->cost)[2] = -1;
				(tt->cost)[3] = -1;
				tt->next = NULL;
				tt->dependants = NULL;
				tt->depth = 0;
				tt->comb = NULL;
			} else if (me == 5 && max < 6) {
				tt = myCalloc(1, sizeof(struct Threat));
				tt->gain = move;
				tt->type = 5;
				(tt->cost) = create();
				(tt->cost)[0] = -1;
				tt->next = NULL;
				tt->dependants = NULL;
				tt->depth = 0;
				tt->comb = NULL;
				return tt;
			} else if (me == 3 && blank > -1 && blank2 > -1 && max < 4 && tt == NULL
				) {
				if (typee[2] == 1 && typee[1] == 1 && typee[4] == 1) {
					p = move + con*(5 - l);
					for (int g = 0; g < 5; g++) {
					//	printf("typee%d: %d\n", g, typee[g]);
					}
					//printf("p = %d\n", p);
					if (isValid(sqrRow, sqrCol, p)) {
						if (board[p] == 0) {
							if (tt == NULL) {
								tt = myCalloc(1, sizeof(struct Threat));
								tt->gain = move;
								tt->type = 3;
								(tt->cost) = create();
								(tt->cost)[0] = blank;
								(tt->cost)[1] = blank2;
								(tt->cost)[2] = p;
							}
						}
					}
				} else if (typee[3] == 1&&typee[1] == 1&&typee[4] == 1) {
					p = move + con*(5 - l);
					if (isValid(sqrRow, sqrCol, p)) {
						if (board[p] == 0) {
							if (tt == NULL) {
								//printf("p = %d\n", p);
								tt = myCalloc(1, sizeof(struct Threat));
								tt->gain = move;
								tt->type = 3;
								(tt->cost) = create();
								(tt->cost)[0] = blank;
								(tt->cost)[1] = blank2;
								(tt->cost)[2] = p;
							}
						}
					}
				} else if (typee[3] == 1&&typee[1] == 1&&typee[2] == 1) {
					p = move + con*(5 - l);
					int q = move + con*(-1 - l);
					if (isValid(sqrRow, sqrCol, p)) {
						if (board[p] == 0) {
							if (tt == NULL) {
								tt = myCalloc(1 , sizeof(struct Threat));
								tt->gain = move;
								tt->type = 3;
								(tt->cost) = create();
								(tt->cost)[0] = blank;
								(tt->cost)[1] = blank2;
								(tt->cost)[2] = p;
							}
							if (isValid(sqrRow, sqrCol, q)) {
								if (board[q] == 0) {
									(tt->cost)[2] = -1;
								}
							}
						} else if (board[p] == 1 + (player % 2)) {
							if (board[q] == 0) {
								if (tt == NULL) {
									tt = myCalloc(1 ,sizeof(struct Threat));
									tt->gain = move;
									tt->type = 3;
									(tt->cost) = create();
									(tt->cost)[0] = blank;
									(tt->cost)[1] = blank2;
									(tt->cost)[2] = q;
								}
							}
						}
					} else if (isValid(sqrRow, sqrCol, q)) {
						if (board[q] == 0) {
							if (tt == NULL) {
								tt = myCalloc(1 ,sizeof(struct Threat));
								tt->gain = move;
								tt->type = 3;
								(tt->cost) = create();
								(tt->cost)[0] = blank;
								(tt->cost)[1] = blank2;
								(tt->cost)[2] = q;
							}
						}
					}
				}
				free(typee);
			}
		};
	};
	return tt;
};

struct Threat *findThreats(int board[], int player, int pot[], int checkables[], int max) {
	//printf("1");
	for (int sqr = 0; sqr < 225; sqr++) {
		if (board[sqr] == player) {
			int sqrRow = sqr % 15;
			int sqrCol = sqr / 15;
			int *blanks = calloc(4 ,sizeof(int));
			for (int move = 1; move < 5; move++) {
				for (int coe = -1; coe < 2; coe += 2) {
					for (int m = 0; m < 4; m++) {
						blanks[m] = 0;
					};
					pot[0] = sqr + move*coe;
					pot[1] = sqr + move*coe*14;
					pot[2] = sqr + move*coe*15;
					pot[3] = sqr + move*coe*16;
					for (int p = 0; p < 4; p++) {
						if (pot[p] >= 0 && pot[p] < 225 && pot[p]/15 - sqrCol < 5 && pot[p]/15 - sqrCol > -5 && (pot[p] % 15) - sqrRow > -5 && (pot[p] % 15) - sqrRow < 5) {
							if(board[pot[p]] == 0) {
								blanks[p]++;
								if (blanks[p] < 3) {
									checkables[pot[p]] = 1;
								};
							} else if(board[pot[p]] != player) {
								blanks[p] ++;
							};
						};
					};
				};
			};
			free(blanks);
		};
	};
	for (int z = 0; z < 225; z++){
		if (checkables[z] == 1) {
			//printf("%d ", z);
		}
	}
	struct Threat *lst = NULL;
	struct Threat *tt = NULL;
	for (int sqr = 0; sqr < 225; sqr++) {
		if (checkables[sqr] == 1) {
			tt = isThreat(sqr, board, player, max);
		};
		if (tt != NULL) {
			tt->next = lst;
			lst = tt;
			tt->depth = 0;
			//tt->dependants = depThreats(board, tt->gain, tt->cost, player, 1);
			//printf("%d ", lst->gain);
		};
		tt = NULL;
		checkables[sqr] = 0;
	};
	return lst;
};

int *dupBoard(int board[]) {
	int *bd = malloc(sizeof(int)*225);
	for (int i = 0; i < 225; i++) {
		bd[i] = board[i];
	}
	return bd;
}

int safe(int bd[], int line[], int *costs[], int types[], int player, int d, int max, int look) {
	int *board = dupBoard(bd);
	for (int i = 0; costs[i] != NULL; i++) {
		for (int j = 0; j < 4; j++) {
			if (costs[i][j] != -1){
				board[costs[i][j]] = 0;
			}
		}
		//printf("%d\n", line[i]);
		board[line[i]] = 0;
	}
	// int *yo = myCalloc(225*sizeof(int));
	// int *yoo = myCalloc(4*sizeof(int));
	// struct Threat *opThreats = findThreats(board, 1 + (player % 2), yoo, yo, max);
	// //free(yo);
	// //free(yoo);
	// printThreats(opThreats, 0);
	// for (struct Threat *curr = opThreats; curr; curr = curr->next) {
	// 	int lol = 1;
	// 	int lol2 = 1;
	// 	if (curr->type == 5) {
	// 		if (lol2) {
	// 			lol2 = 0;
	// 		} else {
	// 			return 0;
	// 		}
	// 		for (int j = 0; j < d + 1; j++) {
	// 			if (curr->gain == line[j] && lol) {
	// 				int temp = line[j]; 
	// 				line[j] = line[0];
	// 				line[0] = temp;
	// 				int *temp2 = costs[j];
	// 				costs[j] = costs[0];
	// 				costs[0] = temp2;
	// 				lol = 0;
	// 				break;
	// 			}
	// 		}
	// 		if (lol) {
	// 			return 0;
	// 		}
	// 	}
	// }
	int bob = 1;
	int unused = 1;
	struct Holder *ans = NULL;
	for (int i = 0; costs[i] != NULL; i++) {
		board[line[i]] = player;
		if (bob == 1) {
			ans = TSS(board, 1 + (player % 2), types[i] + 1);
			if (ans->fEval && unused == 1) {
				for (int j = 0; j < d + 1; j++) {
					//printf("Line:%d %d\n", line[j], look);
					if (look == 0) {
						break;
					}
					for (int k = 0; k < 20; k++) {
						if (((ans->wLine)[k] == line[j] || !comp((ans->wCosts)[k], line[j])) && unused) {
							//printf("%d %d\n", (ans->wLine)[k], line[j]);
							//printf("hi\n");
							int temp = line[j]; 
							line[j] = line[0];
							line[0] = temp;
							int *temp2 = costs[j];
							costs[j] = costs[0];
							costs[0] = temp2;
							unused = 0;
							ans->fEval = 0;
							//printf("haiwefhad\n");
							int yololol = safe(bd, line, costs, types, player, d, max, 0);
							//free(board);
							return yololol;
						}
					}
				}
			}
		}
		if (ans->fEval == 1) {
			bob = 0;
		}

		for (int j = 0; j < 4; j++) {
			if (costs[i][j] != -1){
				board[costs[i][j]] = 1 + (player%2);
			}
		}
	}
	//free(board);
	//printf("BOb: %d\n", bob);
	return bob;
}

struct Threat *depThreats(int board[], int move, int cost[], int typez, int player, int depth, int line[], int *costs[], int types[], int *done, int inD, int max){
	int *arr = myCalloc(5 ,sizeof(int));
	board[move] = player;
	// if (inTable(board)) {
	// 	board[move] = 0;
	// 	return NULL;
	// };
	for (int j = 0; j < 4; j++) {
		if (cost[j] != -1){
			board[cost[j]] = 1 + (player%2);
		}
	}
	line[depth] = move;
	costs[depth] = cost;
	types[depth] = typez;
	struct Threat *tt = NULL;
	int op;
	if (depth < 5) {
		if (player == 1) {
			op = 2;
		} else {
			op = 1;
		};
		int con;
		int p;
		int sqrCol = move/15;
		int sqrRow = move % 15;
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				con = 1;
			} else if (i == 1) {
				con = 14;
			} else if (i == 2) {
				con = 15;
			} else {
				con = 16;
			};
			if (!*done) {
				for (int l = 4; l > -1; l--) {
					int me = 0;
					int you = 0;
					int blank1 = -1;
					int blank2 = -1;
					int blank3 = -1;
					for (int k = 0; k < 5; k++) {
						arr[k] = 0;
					}
					struct Threat *potT = NULL;
					for (int j = 0; j < 5; j++) {
						//printf("%d %d ", me, blank1);
						p = move + con*(j - l);
						if (isValid(sqrRow, sqrCol, p)) {
							if (board[p] == player) {
								me++;
								arr[j] = 1;
							} else if (board[p] == op) {
								break;
							} else if (blank3 > -1 && board[p] == 0) {
								break;
							} else if (blank2 > -1 && board[p] == 0) {
								blank3 = p;
							} else if (blank1 > -1 && board[p] == 0) {
								blank2 = p;
							} else {
								blank1 = p;
							};
						};
					};
					if (me == 4 && blank1 > -1 && max < 6) {
						potT = myCalloc(1 ,sizeof(struct Threat));
						potT->gain = blank1;
						potT->type = 5;
						(potT->cost) = create();
						potT->depth = depth;
						potT->next = tt;
						potT->comb = NULL;
						potT->dependants = NULL;
						tt = potT;
						//printBoard(board);
						// line[depth + 1] = potT->gain;
						// types[depth + 1] = 5;
						if (safe(board, line, costs, types, player, inD, max, 1)) {
							//printf("Player: %d\n", player);
							*done = 1;
							break;
						}
						// } else {
						// 	line[depth + 1] = -1;
						// 	types[depth + 1] = -1;
						// }
					} else if (me == 3 && blank1 > -1 && blank2 > -1 && max < 5) {
						potT = myCalloc(1 ,sizeof(struct Threat));
						potT->type = 4;
						potT->gain = blank1;
						(potT->cost) = create();
						(potT->cost)[0] = blank2;
						potT->depth = depth;
						potT->dependants = NULL;
						potT->comb = NULL;
						potT->next = tt;
						tt = potT;
						potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
						// printf("hiiii\n");
						// printf("%d %d %d %d", depth, *done, blank1, blank2);
						if (*done) {
							break;
						}
						potT = myCalloc(1 ,sizeof(struct Threat));
						potT->gain = blank2;
						potT->dependants = NULL;
						potT->comb = NULL;
						potT->type = 4;
						(potT->cost) = create();
						(potT->cost)[0] = blank1;
						potT->depth = depth;
						potT->next = tt;
						tt = potT;
						potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
						if (*done) {
							break;
						}
					} else if (me == 2 && blank1 > -1 && blank2 > -1 && blank3 > -1 && potT == NULL && max < 4) {
						if (arr[1] == 1 && arr[4] == 1) {
							p = move + con*(5 - l);
							if (isValid(sqrRow, sqrCol, p)) {
								if (board[p] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->type = 3;
									potT->gain = blank2;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[1] = blank3;
									(potT->cost)[2] = p;
									potT->depth = depth;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									// printf("hiiii\n");
									// printf("%d %d %d %d", depth, *done, blank1, blank2);
									if (*done) {
										break;
									}
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank3;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = p;
									(potT->cost)[1] = blank2;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
						} else if (arr[1] == 1 && arr[3] == 1) {
							p = move + con*(5 - l);
							int q = move + con*(-1 - l);
							if (isValid(sqrRow, sqrCol, p) && isValid(sqrRow, sqrRow, q)) {
								if (board[p] == 0 && board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = -1;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} else if (board[p] == 0 && board[q] != 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = p;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} else if (board[p] != 0 && board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = q;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
							if (isValid(sqrRow, sqrCol, p)) {
								if (board[p] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank3;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = p;
									(potT->cost)[1] = blank2;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} 
							}
							if (isValid(sqrRow, sqrCol, q)) {
								if (board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank1;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = q;
									(potT->cost)[1] = blank2;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
						} else if (arr[2] == 1 && arr[3] == 1) {
							p = move + con*(5 - l);
							int q = move + con*(-1 - l);
							if (isValid(sqrRow, sqrCol, p) && isValid(sqrRow, sqrRow, q)) {
								if (board[p] == 0 && board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = -1;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} else if (board[p] == 0 && board[q] != 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = p;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} else if (board[p] != 0 && board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = q;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
							if (isValid(sqrRow, sqrRow, q)) {
								if (board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank1;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank2;
									(potT->cost)[2] = q;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
						} else if (arr[2] == 1 && arr[1] == 1) {
							p = move + con*(5 - l);
							int q = move + con*(-1 - l);
							if (isValid(sqrRow, sqrCol, p), isValid(sqrRow, sqrCol, q)) {
								if (board[p] == 0 && board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = -1;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} else if (board[p] == 0 && board[q] != 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = p;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								} else if (board[p] != 0 && board[q] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank2;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank1;
									(potT->cost)[2] = q;
									(potT->cost)[1] = blank3;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
							if (isValid(sqrRow, sqrCol, p)) {
								if (board[p] == 0) {
									potT = myCalloc(1 ,sizeof(struct Threat));
									potT->gain = blank3;
									potT->dependants = NULL;
									potT->comb = NULL;
									potT->type = 3;
									(potT->cost) = create();
									(potT->cost)[0] = blank2;
									(potT->cost)[2] = p;
									(potT->cost)[1] = blank1;
									(potT->cost)[3] = -1;
									potT->depth = depth;
									potT->next = tt;
									tt = potT;
									potT->dependants = depThreats(board, potT->gain, potT->cost, potT->type, player, depth + 1, line, costs, types, done, inD, max);
									if (*done) {
										break;
									}
								}
							}
						}
					};
					potT = NULL;
				};
			};
		};
	}	
	for (int j = 0; j < 4; j++) {
		if (cost[j] != -1){
			board[cost[j]] = 0;
		}
	}
	if (!*done) {
		//printf("yolo\n");

		costs[depth] = NULL;
		types[depth] = -1;
		line[depth] = -1;
	}
	//free(arr);
	// if (!*done) {
	// 	addToTrans(board);
	// }
	board[move] = 0;
	return tt;

};


int Linee(struct Threat *a, struct Threat *b) {
	int r = a->gain/15 - b->gain/15;
	int c = (a->gain%15) - (b->gain%15);
	if (((r == 0) || (c == 0)  || (c == r || c == -r)) && (-4 < r && r < 4 && -4 < c && c < 4)) {
		return 1;
	};
	return 0;
};

int Line(struct Threat *a, struct Threat *b) {
	for (struct Threat *curr = a; curr; curr = curr->comb) {
		if (!((comp(curr->cost, b->gain) && comp(b->cost, curr->gain) && curr->gain != b->gain && comp2(curr->cost, b->cost)))) {
			return 0;
		};
	};
	for (struct Threat *curr = a; curr; curr = curr->comb) {
		if (Linee(curr, b)) {
			return 1;
		};
	};
	return 0;
};


struct Threat *copyit(struct Threat *copier) {
	struct Threat *copfinal = NULL;
	for (struct Threat *curr = copier; curr; curr = curr->comb) {
		struct Threat *cop = myCalloc(1 ,sizeof(struct Threat));
		cop->gain = curr->gain;
		cop->cost = myCalloc(4 ,sizeof(int));
		for (int i = 0; i < 4; i++) {
			(cop->cost)[i] = (curr->cost)[i];
		}
		cop->depth = curr->depth;
		cop->type = curr->type;
		cop->comb = copfinal;
		cop->next = NULL;
		cop->dependants = NULL;
		copfinal = cop;
	};
	return copfinal;
};

struct Threat *copyitt(struct Threat *copier) {
	struct Threat *copfinal = NULL;
	for (struct Threat *curr = copier; curr; curr = curr->next) {
		struct Threat *cop = myCalloc(1 ,sizeof(struct Threat));
		cop->gain = curr->gain;
		cop->cost = myCalloc(4 ,sizeof(int));
		for (int i = 0; i < 4; i++) {
			(cop->cost)[i] = (curr->cost)[i];
		}
		cop->depth = curr->depth;
		cop->type = curr->type;
		cop->next = copfinal;
		cop->comb = NULL;
		cop->dependants = NULL;
		copfinal = cop;
	};
	//printf("yooo\n");
	return copfinal;
};

void freeThreat(struct Threat *tlist) {
	if(tlist){
		//free(tlist->cost);
		//printf("1\n");
		//freeThreat(tlist->next);
		//printf("2\n");
		//freeThreat(tlist->dependants);
		//printf("3\n");
		//freeThreat(tlist->comb);
		//printf("4\n");
		if (tlist){
			//free(tlist);
		};
	};
};

struct Threat *makecombs(struct Threat *tlist, struct Threat *clist) {
	//printf("hi\n");
	struct Threat *newList = 0;
	for (struct Threat *curr = clist; curr; curr = curr->next) {
		for (struct Threat *adder = tlist; adder; adder = adder->next) {
			if (Line(curr, adder)) {
				struct Threat *bob = copyit(curr);
				struct Threat *bill = myCalloc(1 ,sizeof(struct Threat));
				bill->gain = adder->gain;
				bill->cost = adder->cost;
				bill->depth = adder->depth;
				bill->type = adder->type;
				bill->comb = bob;
				bill->dependants = NULL;
				bob = bill;
				bob->next = newList;
				newList = bob;
			};
		};
	};
	////freeThreat(clist);
	return newList;
};

int eq(struct Threat *a, struct Threat *b, int arr[]) {
	for (struct Threat *curr = a; curr; curr = curr->comb) {
		arr[curr->gain] = 1;
	};
	for (struct Threat *curr = b; curr; curr = curr->comb) {
		if (arr[curr->gain] == 0) {
			for (int i = 0; i < 225; i++) {
				arr[i] = 0;
			};
			return 0;
		} else {
			arr[curr->gain] = 0;
		};
	};
	return 1;
};

void removDups(struct Threat *lst, int lol[]) {
	for (struct Threat *curr = lst; curr->next;) {
		for (struct Threat *curr2 = curr; curr2->next;){
			if (eq(curr2->next, curr, lol)) {
				if (curr2->next->next) {
					struct Threat *bob = curr2->next;
					curr2->next = curr2->next->next;
					//free(bob);
				} else {
					struct Threat *bob = curr2->next;
					curr2->next = NULL;
					//free(bob);
				};
			};
			if (curr2->next) {
				curr2 = curr2->next;
			};
		};
		if (curr->next) {
			curr = curr->next;
		};
	};
};


void TSSh(struct Threat *lst, int board[], int player, int line[], int *done, int *costs[], int types[], int max) {
	int op = 1 + (player % 2);
	for (struct Threat *curr = lst; curr; curr = curr->next) {
		int d = 0;
		struct Threat *lawl = curr;
		for (; lawl->comb; lawl = lawl->comb) {
			board[lawl->gain] = player;
			int *costt = lawl->cost;
			for (int j = 0; j < 4; j++) {
				if (costt[j] != -1){
					board[costt[j]] = op;
				}
			}
			line[d] = lawl->gain;
			costs[d] = lawl->cost;
			types[d] = lawl->type;
			//printf("hiiiiiiiii\n");
			d++;
		};
		//printf("%d %d\n", lawl->gain, lawl->cost);
		if (!*done && lawl->type != 5) {
			lawl->dependants = depThreats(board, lawl->gain, lawl->cost, lawl->type, player, d, line, costs, types, done, d + 1, max);
		} else {
			if (!*done) {
				*done = 1;
				line[d] = lawl->gain;
				types[d] = lawl->type;
				//printf("Gain = %d and Type = %d\n", lawl->gain, lawl->type);
			}
		}
		for (struct Threat *llawl = curr; llawl->comb; llawl = llawl->comb) {
			board[llawl->gain] = 0;
			int *cost = llawl->cost;
			for (int j = 0; j < 4; j++) {
				if (cost[j] != -1){
					board[cost[j]] = 0;
				}
			}
			if (!*done) {
				line[d] = -1;
				costs[d] = NULL;
				types[d] = -1;	
			}
			d--;
		};
	};
};

struct Holder *TSS(int board[], int player, int max) {
	//int predone = inTSTable(board);
	//printf("predone %d\n", predone);
//	struct Threat *threatzz = NULL;
//	struct Threat *com;
//	printf("yolo\n");
	int tExists = 1;
	//struct Threat *comT;
	//threatz = findThreats(board, player, pots, dontTouch);
	int *done = myCalloc(1 ,sizeof(int));
	*done = 0;
	int *line = myCalloc(20 ,sizeof(int));
	int *(*costs) = myCalloc(20 ,sizeof(int*));
	int *types = calloc(20 ,sizeof(int));
	int *compArray = calloc(225 ,sizeof(int));
	int *dontTouch = calloc(225 ,sizeof(int));
	int *pots = calloc(4 ,sizeof(int));
	for (int i = 0; i < 20; i++) {
		// printf("%d\n", i);
		// printf("line\n");
		line[i] = -1;
		//printf("cost\n");
		costs[i] = NULL;
		//printf("type\n");
		types[i] = -1;
	}
	// if (predone) {
	// 	struct Holder *ans = myCalloc(1 ,sizeof(struct Holder));
	// 	ans->fEval = *done;
	// 	ans->wLine = line;
	// 	ans->wCosts = costs;
	// 	return ans;	
	// }
	struct Threat *bob = findThreats(board, player, pots, dontTouch, max);
	struct Threat *threatzz = 0;
	struct Threat *comT = 0;

	// = myCalloc(1 ,sizeof(struct Threat));
	while (tExists) {
		if (threatzz == NULL) {;
			threatzz = bob;
			comT = threatzz;
		} else {
			comT = makecombs(threatzz, comT);
		};
		if (comT != NULL) {
			removDups(comT, compArray);
			//printThreats(comT, 0);
			TSSh(comT, board, player, line, done, costs, types, max);
		} else {
		 	tExists = 0;
		};
		//printThreats(comT, 0);
		if (*done == 1) {
			tExists = 0;
		}
	};
	// if (*done) {
	// 	for (int i = 0; i<20; i++) {
	// 		if (costs[i] != NULL){
	// 			printf("%d %d %d %d %d %d\n", line[i], costs[i][0], costs[i][1], costs[i][2], costs[i][3], types[i]);	
	// 		} else {
	// 			printf("%d %d %d\n", line[i], 0, types[i]);	
	// 		}
	// 	}
	// }
	// if (!(*done)) {
	// 	addToTSTrans(board);
	// }
	struct Holder *ans = myCalloc(1 ,sizeof(struct Holder));
	ans->fEval = *done;
	ans->wLine = line;
	ans->wCosts = costs;
	free(types);
	free(compArray);
	free(dontTouch);
	free(pots);
	return ans;
};

int *combine(int line[], int* cost[]) {
	int *lol = myCalloc(100 ,sizeof(int));
	int i = 0;
		while (line[i] != -1) {
			lol[i] = line[i];
			i++;
		}
		for (int j = 0; j < 20; j++) {
			if(cost[j] != 0) {
				for (int z = 0; z < 4; z++) {
					if (cost[j][z] != -1) {
						lol[i] = cost[j][z];
						i++;
						//printf("%d\n", i);
					}
				}
			}
		}
		for (; i < 100; i++) {
			//printf("%d\n", i);
			lol[i] = -1;
		}
		
	return lol;
}


int analyze(int move, int board[], int player) {
	int score = 0;
	int con = 0;
	int p = 0;
	int sqrCol = move/15;
	int sqrRow = move % 15;
	int *arr = calloc(5 ,sizeof(int));
	int op = 1 + (player % 2);
	//printf("hi\n");
	for (int i = 0; i < 4; i++) {
		//printf("hi2\n");

		if (i == 0) {
			con = 1;
		} else if (i == 1) {
			con = 14;
		} else if (i == 2) {
			con = 15;
		} else {
			con = 16;
		};
		for (int l = 4; l > -1; l--) {
			//printf("hi3\n");
			int me = 1;
			int you = 0;
			int blanks = 0;
			for (int k = 0; k < 5; k++) {
				arr[k] = 0;
			}
			for (int j = 0; j < 5; j++) {

				//printf("%d %d ", me, blank1);
				p = move + con*(j - l);
				if (isValid(sqrRow, sqrCol, p)) {
					if (board[p] == player) {
						me++;
						arr[j] = 1;
					} else if (board[p] == op) {
						you++;
						arr[j] = 1;
					} else if (board[p] == 0) {
						blanks ++;
					}
				};
			}
			blanks--;
			//printf("%d %d %d\n", me, you, blanks);
			if (me == 5) {
				score += 1000000;
			} else if (you == 4 && me == 1) {
				score += 100000;
			} else if (you == 3 && me == 1 &&blanks == 1) {
				score += 8000;
			} else if (you == 2 && me == 1 &&blanks == 2) {
				score += 4000;
			} else if (you == 1 && me == 1 &&blanks == 3) {
				//printf("yo\n");
				score += 90;
			} else if (you == 0) {
				if (me == 4 && blanks == 1) {
					score += 10000;
				} else if (me == 3 && blanks == 2) {
					score += 7000;
				} else if (me == 2 && blanks == 3) {
					//printf("hii\n");
					score += 3000;
				} else if (me == 1 && blanks == 4) {
					//printf("hi\n");
					score += 70;
				}
				if(con == 14 || con == 16) {
					score += 1;
				}
			};
		};

	};
	free(arr);
	return score;
}

void insertInto(int scrs[], int mvs[], int scr, int mv, int n) {
	if (scrs[n-1] < scr) {
		scrs[n-1] = scr;
		mvs[n-1] = mv;
		for (int i = n-1; i > 0; i--) {
			if (scrs[i] > scrs[i - 1]) {
				int temp = scrs[i];
				scrs[i] = scrs[i-1];
				scrs[i-1] = temp;
				temp = mvs[i];
				mvs[i] = mvs[i-1];
				mvs[i-1] = temp;
			} else {
				break;
			}
		}
	}
}

int evalBoard(int board[], int player) {
	int score = 0;
	int pos = player*2 - 3;
	int *checks = calloc(225, sizeof(int));
	int *pots = calloc(4, sizeof(int));
	struct Threat *myThreats = findThreats(board, player, pots, checks, 3);
	struct Threat *yourThreats = findThreats(board, 1 + (player % 2), pots, checks, 3);
	for (; yourThreats; yourThreats = yourThreats->next) {
		score -= pos*100;
	}
	for (; myThreats; myThreats = myThreats->next) {
		score += pos*100;
	}
	free(checks);
	free(pots);
	return score;
}

int *bmoves(int n, int board[], int player, int checks[], int move) {
	int *bestMoves = calloc(n, sizeof(int));
	int *scrToBeat = calloc(n, sizeof(int));
	int *pots = calloc(4, sizeof(int));
	if (move != -1) {
			struct Threat *daMove = isThreat(move, board, player % 2 + 1, 3);
	// struct Threat *threat = findThreats(board, player, pots, checks, 3);
		if (daMove != NULL) {
			for (int i = 0; i < 3; i++ ) {
			int move = daMove->cost[i];
			if (move != -1) {
				int scre = 2000000;
				insertInto(scrToBeat, bestMoves, scre, move, n);
				checks[move] = 1;
			}	
		}	
		}
		
	}
	free(pots);
	int j = 0;
	for (int i = 0; i < 225; i++) {
		if (board[i] == 0 && checks[i]==0) {
			int scr = 0;
			// board[i] = player;
			// struct Holder *yo = TSS(board, player, 3);
			// board[i] = 0;
			// //freeHolder(yo);
			scr = analyze(i, board, player);
			// if (yo->fEval == 1) {
			//  	scr += 100000;
			//  	printf("I have a hidden Threat\n");
			// }
			//scr += checks[i]*100000;
			//printf("yo\n");
			//printf("%d\n", scr);
			//printf("MOVES: %d %d\n", bestMove, i);
			insertInto(scrToBeat, bestMoves, scr, i, n);
		}
	}
	free(scrToBeat);
	return bestMoves;
}
struct Node *move = 0;
int minimax(int board[], int player, int depth, int maxDepth, int nums, int a, int b, int prev) {
	// if (depth > 0) {
	// 	int scoree = inTable(board);
	// 	if (scoree) {
	// 		//printf("DID IT!!!\n");
	// 		return scoree;
	// 	}
	// }
	int bestMove = -1;
	int bestScore = 0;
	struct Node *bestLine = 0;
	int *checks = calloc(225, sizeof(int));
	// int *pots = myCalloc(4, sizeof(int));
	// struct Threat *opThreats =  findThreats(board, 1 + (player % 2), pots, checks, 3);
	// if (opThreats != NULL) {
	// 	for (; opThreats; opThreats = opThreats->next) {
	// 		checks[opThreats->gain]++;
	// 		for (int k = 0; k < 4; k++) {
	// 			if ((opThreats->cost)[k] != -1) {
	// 				checks[(opThreats->cost)[k]]++;
	// 			}
	// 		}
	// 	}
	// }
	// if (opThreats != NULL) {
	// 	for (; opThreats; opThreats = opThreats->next) {
	// 		checks[opThreats->gain] += 1;
	// 		for (int k = 0; k < 4; k++) {
	// 			if ((opThreats->cost)[k] != -1) {
	// 				checks[(opThreats->cost)[k]] += 1;
	// 			}
	// 		}
	// 	}
	// }
	int *lst = bmoves(nums, board, player, checks, prev);
	// if (depth == 0) {
	// 	for (int z = 0; z < nums; z++) {
	// 		printf("Best Moves: %d\n", lst[z]);
	// 	}
	// }
	//free(checks);
	//free(pots);
	for (int i = 0; i < nums; i++) {
		int score = 0;
		struct Node *zline = 0;
		board[lst[i]] = player;
		if (depth < maxDepth) {
			struct Holder *threats= TSS(board, 1 + (player % 2), 3);
			if (threats->fEval) {
				//printf("DID IT MOFOZ\n");
				zline = ccons(lst[i], 0);
				score = -1*((player*2) -3)*1000000;

			} else {
				if (i > 0) {
					if (player == 1) {
						score = minimax(board, 1 + (player % 2), depth + 1, maxDepth, nums, a, bestScore, lst[i]);
					} else {
						score = minimax(board, 1 + (player % 2), depth + 1, maxDepth, nums, bestScore, b, lst[i]);
					}
				} else {
					score = minimax(board, 1 + (player % 2), depth + 1, maxDepth, nums, a, b, lst[i]);					
				}
				// printf("%d", move);
				zline = ccons(lst[i], move);
			}
		} else {
			score = evalBoard(board, 1 + (player % 2));
		}
		if (bestMove == -1 || (score > bestScore && player == 2) || (score < bestScore && player == 1)) {
			//printf("Move changed\n");
			bestScore = score;
			bestMove = lst[i];
			freeLst(bestLine);
			bestLine = zline;
		} else {
			freeLst(zline);
		}
		board[lst[i]] = 0;
		if (a > b) {
			break;
		}
	}
	free(checks);
	currptr = initptr;
	currptr2 = initptr2;
	currptr3 = initptr3;
	free(lst);
	if (depth == 0) {
		// printf("hi\n");
		// 		printf("SCORE = %d\n", bestScore);
		int k = 0;
		for (;bestLine; bestLine = bestLine->rest) {
			// printf("%d\n", bestLine->rest);
			best_line[k] = bestLine->first;
			k ++;
		}
		best_line[depth] = bestMove;
		board_score = bestScore;
		return bestMove;
	} else {
		addToTrans(board, bestScore);
		move = bestLine;
		if (bestScore > 0) {
			return bestScore - depth;;
		} else if (bestScore < 0) {
			return bestScore + depth;
		} else {
			return bestScore;
		}

	}
}

int aiMove(int board[], int player) {
	int op = 1 + (player % 2);
	struct Holder *myFive = TSS(board, player, 5);
	if (myFive->fEval == 1) {
			// printf("I Win\n");
			// for (int i = 0; i<10;i++) {
			// 	printf("Move: %d\n", (myFive->wLine)[i]);
			// }
		return (myFive->wLine)[0];
	}
	//freeHolder(myFive);
	struct Holder *urFive = TSS(board, op, 5);
	if (urFive->fEval == 1) {
		// printf("Defending His Win\n");
		return (urFive->wLine)[0];
	}
	// struct Holder *myrFive = TSS(board, player, 4);
	// if (myrFive->fEval == 1) {
	// 		// printf("I Win\n");
	// 		// for (int i = 0; i<10;i++) {
	// 		// 	printf("Move: %d\n", (myFive->wLine)[i]);
	// 		// }
	// 	return (myrFive->wLine)[0];
	// }
	// //freeHolder(myFive);
	// struct Holder *urrFive = TSS(board, op, 4);
	// if (urrFive->fEval == 1) {
	// 	// printf("Defending His Win\n");
	// 	return (urrFive->wLine)[0];
	// }
	//freeHolder(urFive);
	struct Holder *myWin = TSS(board, player, 3);
	if (myWin->fEval == 1) {
		// printf("I have a win\n");
		// for (int i = 0; i<10;i++) {
		// 	printf("Move: %d\n", (myWin->wLine)[i]);
		// }
		return (myWin->wLine)[0];
	}
	//freeHolder(myWin);
	// struct Holder *yourWin = TSS(board, op, 3);
	// if (yourWin->fEval == 1) {
	// 	printf("He Has a win\n");
	// 	int *possibles = combine(yourWin->wLine, yourWin->wCosts);
	// 	int bestMove = possibles[0];
	// 	int bestScore = 0;
	// 	for (int y = 0; y < 100; y++) {
	// 		if (possibles[y] != -1) {
	// 			board[possibles[y]] = player;
	// 			struct Holder *otherWin = TSS(board, op, 3);
	// 			board[possibles[y]] = 0;
	// 			if (otherWin->fEval == 0) {
	// 				int bob = analyze(possibles[y], board, player);
	// 				if (bob > bestScore) {
	// 					printf("DEFENDED MOFOZ\n");
	// 					bestScore = bob;
	// 					bestMove = possibles[y];
	// 				}
	// 			}
	// 			//freeHolder(otherWin);
	// 		}
	// 	}
	// 	return bestMove;
	// }
	// //freeHolder(yourWin);
	// int *pots = myCalloc(4, sizeof(int));
	// int *checks = myCalloc(225, sizeof(int));
	// struct Threat *opThreats =  findThreats(board, op, pots, checks, 3);
	// if (opThreats != NULL) {
	// 	for (; opThreats; opThreats = opThreats->next) {
	// 		checks[opThreats->gain]++;
	// 		for (int k = 0; k < 4; k++) {
	// 			if ((opThreats->cost)[k] != -1) {
	// 				checks[(opThreats->cost)[k]]++;
	// 			}
	// 		}
	// 	}
	// }
	// if (opThreats != NULL) {
	// 	for (; opThreats; opThreats = opThreats->next) {
	// 		checks[opThreats->gain] += 1;
	// 		for (int k = 0; k < 4; k++) {
	// 			if ((opThreats->cost)[k] != -1) {
	// 				checks[(opThreats->cost)[k]] += 1;
	// 			}
	// 		}
	// 	}
	// }
	// printf("HIIII\n");
	turn ++;
	if (turn < 3) {
		return minimax(board, 2, 0, 5, 5, -20000000, 20000000, -1);	
	} else {
		return minimax(board, 2, 0, 5, 5, -20000000, 20000000, -1);
	}
}

void print_variation(int board[], int move) {
	int *copied = myCalloc(225, 4);
	for (int i = 0; i < 225; i++) {
		copied[i] = board[i];
	}
	if (best_line[0] == -1) {
		copied[move] = 0;
		struct Holder *myWin = TSS(copied, 2, 3);
		for (int i = 0; i < 20; i++) {
		if ((myWin->wLine)[i] != -1) {
			fprintf(stdout, "%d ", (myWin->wLine)[i]);
		}
		if ((myWin->wCosts)[i] != NULL) {
			fprintf(stdout, "%d ", (myWin->wCosts)[i][0]);
		}
	}
	} else {
		for (int i = 0; i < 10; i++) {
		if (best_line[i] == -1) {
			break;
		}
		fprintf(stdout, "%d ", best_line[i]);
		if (i % 2 == 0) {
			copied[best_line[i]] = 2;
		} else {
			copied[best_line[i]] = 1;
		}
		best_line[i] = -1;
	}
	struct Holder *myWin = TSS(copied, 2, 3);
	for (int i = 0; i < 20; i++) {
		if ((myWin->wLine)[i] != -1) {
			fprintf(stdout, "%d ", (myWin->wLine)[i]);
		}
		if ((myWin->wCosts)[i] != NULL) {
			fprintf(stdout, "%d ", (myWin->wCosts)[i][0]);
		}
	}
	}
	
}

void runGame() {
	int *board = calloc(225 ,sizeof(int));
	for (int i = 0; i < 225; i++) {
		board[i] = 0;
	}
	int fp = 0;
	scanf("%d", &fp);
	if (fp) {
		board[112] = 2;
	}
	// printBoard(board);
	while (1) {
		// printf("%d\n", currptr);
		int move = 0;
		int myMove = 0;
		scanf("%d", &move);
		if (move == 333) {
			break;
		}
		if (move < 225 && move > -1) {
			// for (int j = 0; j < 50; j++) {
			// 	printf("\n");
			// }
			if (board[move] == 0) {
				board[move] = 1;
				myMove = aiMove(board, 2);
				board[myMove] = 2;
				fprintf(stdout, "%d ", myMove);
				fprintf(stdout, "%d ", board_score);
				print_variation(board, myMove);
				fprintf(stdout, "%c\n", 'n');
				// printBoard(board);
				// printf("boardEval: %d\n", evalBoard(board, 1));
			}
		}
	currptr = initptr;
	currptr2 = initptr2;
	currptr3 = initptr3;
	transptr = initTransptr;
	memset(transtable, 0, 800000);
	fflush(stdout);
	}
	//free(board);
}

int main() {
	initptr = malloc(1024*1024*1024);
	currptr = initptr;
	maxptr = currptr + 1024*1024*1024/4;

	initptr2 = malloc(1024*1024*1024);
	currptr2 = initptr2;
	maxptr2 = currptr2 + 1024*1024*1024/4;

	initptr3 = malloc(1024*1024*1024);
	currptr3 = initptr3;
	maxptr3 = currptr3 + 1024*1024*1024/4;

	transtable = calloc(100000, 8);
	transptr = malloc(100000*228*4);
	initTransptr = transptr;
	
	tstranstable = calloc(100000, 8);
	tstransptr = malloc(100000*228*4);
	tsinitTransptr = tstransptr;
	
	best_line = calloc(10, sizeof(int));
	for (int i = 0; i < 10; i++) {
		best_line[i] = -1;
	}
	// printf("transtable: %d\n", transtable);
	// int *board = myCalloc(1 ,sizeof(int)*225);
	// //struct Node *black = cons(111, cons(142, cons(158, cons(159, cons(129, 0)))));
	// //struct Node *white = cons(126, cons(141, cons(156, cons(128, cons(145, 0)))));
	// // struct Node *black = cons(97, cons(98, cons(99, cons(98+15, cons(128, cons(127, cons(129, cons(127+15, cons(157, cons(156, cons(158,  0)))))))))));
	// // struct Node *white = cons(96, cons(96+15, cons(96+30, cons(96+45, cons(96+60-1, cons(97+75, cons(98+45, cons(145, cons(130, cons(114, cons(97+15, 0)))))))))));
	// // // struct Node *black = cons(47, cons(49, cons(62, cons(66, cons(80, cons(95, cons(96, cons(92, cons(96+16, cons(96+32, 0))))))))));
	// // // struct Node *white = cons(50, cons(65, cons(64, cons(63, cons(78, cons(77, cons(81, cons(97, cons(98, cons(98+46, cons(4, cons(1, cons(2, cons(220, 0))))))))))))));
	// //struct Node *white = cons(113, 0);
	// //loadBoard(white, black, board);
	// // printBoard(board);
	// board[112] = 1;
	// aiMove(board, 2);
	// // printBoard(board);
	// int player = 2;
	// int lev = 3;
	// printf("final eval: %d", (TSS(board, player, 3)->fEval));
	// //free(board);
	//printThreatsC(comT, 0);
	runGame();
	return 0;
};
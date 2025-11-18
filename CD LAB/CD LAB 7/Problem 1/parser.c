#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID 1
#define EQ 2
#define AST 3
#define DOLLAR 4

#define MAXP 20
#define MAXSYM 50
#define MAXS 200
#define MAXITEMS 200

typedef struct { char *lhs; char *rhs[10]; int rhs_len; } Prod;
Prod prods[MAXP]; int P=0;

typedef struct { char *name; } Sym;

char *symlist[50]; int symcount=0;

typedef struct { int p; int dot; } Item;
typedef struct { Item items[MAXITEMS]; int n; } State;
State states[MAXS]; int S_count=0;

int same_item(Item *a, Item *b){ return a->p==b->p && a->dot==b->dot; }
int state_equal(State *a, State *b){
    if(a->n!=b->n) return 0;
    for(int i=0;i<a->n;i++){
        int found=0;
        for(int j=0;j<b->n;j++) if(same_item(&a->items[i],&b->items[j])){ found=1; break; }
        if(!found) return 0;
    }
    return 1;
}

void add_item(State *st, Item it){
    for(int i=0;i<st->n;i++) if(same_item(&st->items[i],&it)) return;
    st->items[st->n++]=it;
}

int is_nonterm(char *s){ return (s[0]>='A' && s[0]<='Z'); }
char *symbol_after_dot(Item *it){ if(it->dot < prods[it->p].rhs_len) return prods[it->p].rhs[it->dot]; return NULL; }

void add_prod(char *A, char *rhs_tokens[], int len){
    prods[P].lhs = strdup(A);
    prods[P].rhs_len = len;
    for(int i=0;i<len;i++) prods[P].rhs[i] = strdup(rhs_tokens[i]);
    P++;
}

int sym_idx(char *s){ for(int i=0;i<symcount;i++) if(!strcmp(symlist[i],s)) return i; return -1; }

void build_symbol_list(){
    symcount=0;
    for(int i=0;i<P;i++){
        if(sym_idx(prods[i].lhs)==-1) symlist[symcount++]=prods[i].lhs;
        for(int j=0;j<prods[i].rhs_len;j++) if(sym_idx(prods[i].rhs[j])==-1) symlist[symcount++] = prods[i].rhs[j];
    }
    if(sym_idx("$")==-1) symlist[symcount++] = "$";
}

void closure(State *I){
    int changed=1;
    while(changed){ changed=0;
        for(int i=0;i<I->n;i++){
            Item it = I->items[i];
            char *sym = symbol_after_dot(&it);
            if(sym && is_nonterm(sym)){
                for(int k=0;k<P;k++) if(!strcmp(prods[k].lhs,sym)){
                    Item nit; nit.p=k; nit.dot=0;
                    int found=0; for(int j=0;j<I->n;j++) if(same_item(&I->items[j],&nit)){ found=1; break; }
                    if(!found){ add_item(I,nit); changed=1; }
                }
            }
        }
    }
}

void goto_state(State *I, char *X, State *J){
    J->n=0;
    for(int i=0;i<I->n;i++){
        Item it = I->items[i];
        char *sym = symbol_after_dot(&it);
        if(sym && !strcmp(sym,X)){ Item nit = it; nit.dot++; add_item(J,nit); }
    }
    closure(J);
}

int find_state(State *st){ for(int i=0;i<S_count;i++) if(state_equal(st,&states[i])) return i; return -1; }

void build_canonical(){
    State I0; I0.n=0; Item it; it.p=0; it.dot=0; add_item(&I0,it); closure(&I0);
    states[0]=I0; S_count=1;
    int changed=1;
    while(changed){ changed=0;
        for(int i=0;i<S_count;i++){
            for(int s=0;s<symcount;s++){
                State J; goto_state(&states[i], symlist[s], &J);
                if(J.n>0){
                    int idx = find_state(&J);
                    if(idx==-1){ states[S_count]=J; S_count++; changed=1; }
                }
            }
        }
    }
}

char *first_set[50][50]; int first_n[50];
char *follow_set[50][50]; int follow_n[50];

int is_terminal_sym(char *s){ return !is_nonterm(s) && strcmp(s,"$")!=0; }

void compute_FIRST(){
    for(int i=0;i<symcount;i++) first_n[i]=0;
    int changed=1;
    while(changed){ changed=0;
        for(int p=0;p<P;p++){
            int A = sym_idx(prods[p].lhs);
            if(prods[p].rhs_len==0) continue;
            char *X = prods[p].rhs[0];
            int xi = sym_idx(X);
            if(is_terminal_sym(X)){
                int found=0; for(int k=0;k<first_n[A];k++) if(!strcmp(first_set[A][k],X)) found=1;
                if(!found){ first_set[A][first_n[A]++]=X; changed=1; }
            } else {
                for(int k=0;k<first_n[xi];k++){
                    int found=0; for(int m=0;m<first_n[A];m++) if(!strcmp(first_set[A][m], first_set[xi][k])) found=1;
                    if(!found){ first_set[A][first_n[A]++]=first_set[xi][k]; changed=1; }
                }
            }
        }
    }
}

void compute_FOLLOW(){
    for(int i=0;i<symcount;i++) follow_n[i]=0;
    int Sidx = sym_idx(prods[1].lhs);
    follow_set[Sidx][follow_n[Sidx]++] = strdup("$");
    int changed=1;
    while(changed){ changed=0;
        for(int p=0;p<P;p++){
            for(int i=0;i<prods[p].rhs_len;i++){
                char *B = prods[p].rhs[i]; if(!is_nonterm(B)) continue;
                if(i+1 < prods[p].rhs_len){
                    char *beta = prods[p].rhs[i+1]; int beta_idx = sym_idx(beta); int Bidx = sym_idx(B);
                    if(is_terminal_sym(beta)){
                        int found=0; for(int k=0;k<follow_n[Bidx];k++) if(!strcmp(follow_set[Bidx][k],beta)) found=1;
                        if(!found){ follow_set[Bidx][follow_n[Bidx]++]=beta; changed=1; }
                    } else {
                        for(int k=0;k<first_n[beta_idx];k++){
                            int found=0; for(int m=0;m<follow_n[Bidx];m++) if(!strcmp(follow_set[Bidx][m], first_set[beta_idx][k])) found=1;
                            if(!found){ follow_set[Bidx][follow_n[Bidx]++]=first_set[beta_idx][k]; changed=1; }
                        }
                    }
                } else {
                    int Aidx = sym_idx(prods[p].lhs); int Bidx = sym_idx(B);
                    for(int k=0;k<follow_n[Aidx];k++){
                        int found=0; for(int m=0;m<follow_n[Bidx];m++) if(!strcmp(follow_set[Bidx][m], follow_set[Aidx][k])) found=1;
                        if(!found){ follow_set[Bidx][follow_n[Bidx]++]=follow_set[Aidx][k]; changed=1; }
                    }
                }
            }
        }
    }
}

typedef struct { char act[16]; int num; } Action;
Action ACTION_LR0[MAXS][50]; int GOTO_LR0[MAXS][50];
Action ACTION_SLR[MAXS][50]; int GOTO_SLR[MAXS][50];

int term_index[50]; int term_count=0;
int nonterm_index[50]; int nonterm_count=0;

void classify_symbols(){
    term_count=0; nonterm_count=0;
    for(int i=0;i<symcount;i++){
        if(!strcmp(symlist[i],"$")) { term_index[term_count++]=i; continue; }
        if(is_nonterm(symlist[i])) nonterm_index[nonterm_count++]=i; else term_index[term_count++]=i;
    }
    int dollar_pos=-1; for(int i=0;i<term_count;i++) if(!strcmp(symlist[term_index[i]],"$")) { dollar_pos=i; break; }
    if(dollar_pos!=-1 && dollar_pos!=term_count-1){ int tmp=term_index[dollar_pos]; for(int k=dollar_pos;k<term_count-1;k++) term_index[k]=term_index[k+1]; term_index[term_count-1]=tmp; }
}

void build_tables(){
    for(int i=0;i<S_count;i++) for(int j=0;j<symcount;j++){
        ACTION_LR0[i][j].act[0]=0; ACTION_SLR[i][j].act[0]=0;
        GOTO_LR0[i][j]=GOTO_SLR[i][j]=-1;
    }
    for(int i=0;i<S_count;i++){
        for(int k=0;k<states[i].n;k++){
            Item it = states[i].items[k]; char *a = symbol_after_dot(&it);
            if(a){
                int Xidx = sym_idx(a);
                State J; goto_state(&states[i], a, &J);
                int jidx = find_state(&J);
                if(jidx!=-1){
                    if(is_terminal_sym(a)){
                        strcpy(ACTION_LR0[i][Xidx].act,"s"); ACTION_LR0[i][Xidx].num=jidx;
                        strcpy(ACTION_SLR[i][Xidx].act,"s"); ACTION_SLR[i][Xidx].num=jidx;
                    } else {
                        GOTO_LR0[i][Xidx]=jidx; GOTO_SLR[i][Xidx]=jidx;
                    }
                }
            } else {
                if(it.p==0){
                    int doll = sym_idx("$"); strcpy(ACTION_LR0[i][doll].act,"acc"); strcpy(ACTION_SLR[i][doll].act,"acc");
                } else {
                    for(int t=0;t<symcount;t++) if(is_terminal_sym(symlist[t]) || !strcmp(symlist[t],"$")){
                        if(ACTION_LR0[i][t].act[0]){
                            if(strchr(ACTION_LR0[i][t].act,'/')){
                                char buf[16]; snprintf(buf,sizeof(buf),"r%d", it.p);
                                if(strlen(ACTION_LR0[i][t].act)+strlen(buf)+1 < (int)sizeof(ACTION_LR0[i][t].act)){
                                    strcat(ACTION_LR0[i][t].act, "/"); strcat(ACTION_LR0[i][t].act, buf);
                                }
                            } else {
                                char existing[16]; strcpy(existing, ACTION_LR0[i][t].act);
                                char buf[16]; snprintf(buf,sizeof(buf),"r%d", it.p);
                                snprintf(ACTION_LR0[i][t].act,sizeof(ACTION_LR0[i][t].act),"%s/%s", existing, buf);
                            }
                        } else { strcpy(ACTION_LR0[i][t].act,"r"); ACTION_LR0[i][t].num = it.p; }
                    }
                    int Aidx = sym_idx(prods[it.p].lhs);
                    for(int f=0; f<follow_n[Aidx]; f++){
                        char *term = follow_set[Aidx][f]; int tid = sym_idx(term);
                        if(tid==-1) continue;
                        if(ACTION_SLR[i][tid].act[0]){
                            if(strchr(ACTION_SLR[i][tid].act,'/')){
                                char buf[16]; snprintf(buf,sizeof(buf),"r%d", it.p);
                                if(strlen(ACTION_SLR[i][tid].act)+strlen(buf)+1 < (int)sizeof(ACTION_SLR[i][tid].act)){
                                    strcat(ACTION_SLR[i][tid].act, "/"); strcat(ACTION_SLR[i][tid].act, buf);
                                }
                            } else {
                                char existing[16]; strcpy(existing, ACTION_SLR[i][tid].act);
                                char buf[16]; snprintf(buf,sizeof(buf),"r%d", it.p);
                                snprintf(ACTION_SLR[i][tid].act,sizeof(ACTION_SLR[i][tid].act),"%s/%s", existing, buf);
                            }
                        }
                        else { strcpy(ACTION_SLR[i][tid].act,"r"); ACTION_SLR[i][tid].num=it.p; }
                    }
                }
            }
        }
    }
}

void print_state(State *st){ 
    for(int i=0;i<st->n;i++){
        Item it = st->items[i];
        printf(" (%d) %s ->", it.p, prods[it.p].lhs);
        for(int k=0;k<prods[it.p].rhs_len;k++){
            if(k==it.dot) printf(" .");
            printf(" %s", prods[it.p].rhs[k]);
        }
        if(it.dot==prods[it.p].rhs_len) printf(" .");
        printf("\n");
    }
}

static void print_one_table(const char *title, Action ACTION[][50], int GOTO[][50]){
    printf("\n%s\n", title);
    printf("%-6s |","State");
    for(int i=0;i<term_count;i++){
        char *sym = symlist[term_index[i]];
        printf(" %4s", sym);
    }
    printf(" ||");
    int shown_nt=0;
    for(int i=0;i<nonterm_count;i++){
        char *sym = symlist[nonterm_index[i]];
        if(strchr(sym,'\'')) continue;
        printf(" %4s", sym); shown_nt++;
    }
    printf("\n");
    int width = 7 + term_count*5 + 2 + shown_nt*5;
    for(int i=0;i<width;i++) putchar('-');
    putchar('\n');
    for(int st=0; st<S_count; st++){
        printf("%-6d |", st);
        for(int i=0;i<term_count;i++){
            int idx = term_index[i];
            if(ACTION[st][idx].act[0]){
                if(!strcmp(ACTION[st][idx].act,"s")) printf(" s%-3d", ACTION[st][idx].num);
                else if(!strcmp(ACTION[st][idx].act,"r")) printf(" r%-3d", ACTION[st][idx].num);
                else if(!strcmp(ACTION[st][idx].act,"acc")) printf(" acc ");
                else if(strchr(ACTION[st][idx].act,'/')){
                    int len = (int)strlen(ACTION[st][idx].act);
                    if(len<=5) printf(" %-4s", ACTION[st][idx].act);
                    else { char tmp[6]; strncpy(tmp,ACTION[st][idx].act,5); tmp[5]='\0'; printf(" %-4s", tmp); }
                }
                else printf(" %-4s", ACTION[st][idx].act);
            } else printf("     ");
        }
        printf(" ||"); shown_nt=0;
        for(int i=0;i<nonterm_count;i++){
            int idx = nonterm_index[i]; char *sym = symlist[idx];
            if(strchr(sym,'\'')) continue;
            if(GOTO[st][idx]!=-1) printf(" %4d", GOTO[st][idx]); else printf("     ");
        }
        printf("\n");
    }
}

void print_tables(){
    classify_symbols();
    print_one_table("LR(0) TABLE (ACTION | GOTO)", ACTION_LR0, GOTO_LR0);
    print_one_table("SLR(1) TABLE (ACTION | GOTO)", ACTION_SLR, GOTO_SLR);
}

void report_conflicts(){
    printf("\nConflict summary:\n");
    int resolved=0, unresolved=0;
    for(int i=0;i<S_count;i++){
        for(int t=0;t<symcount;t++) if(is_terminal_sym(symlist[t]) || !strcmp(symlist[t],"$")){
            int lr0_conf = ACTION_LR0[i][t].act[0] && strchr(ACTION_LR0[i][t].act,'/');
            int slr_conf = ACTION_SLR[i][t].act[0] && strchr(ACTION_SLR[i][t].act,'/');
            if(lr0_conf && !slr_conf){
                if(!resolved) printf("  Resolved (present in LR(0), absent in SLR(1)):\n");
                printf("    State %d on %s -> %s\n", i, symlist[t], ACTION_LR0[i][t].act);
                resolved=1;
            } else if(lr0_conf && slr_conf){
                if(!unresolved) printf("  Unresolved (still conflicting under SLR(1)):\n");
                printf("    State %d on %s -> %s\n", i, symlist[t], ACTION_SLR[i][t].act);
                unresolved=1;
            }
        }
    }
    if(!resolved) printf("  No LR(0) conflicts resolved by SLR(1) for this grammar.\n");
    if(!unresolved) printf("  No remaining SLR(1) conflicts (grammar would be SLR(1)).\n");
    else printf("\nNote: Presence of unresolved conflicts shows grammar is not SLR(1); canonical LR(1) lookahead would separate shift and reduce here.\n");
}

void parse_with_table(Action ACTION[][50], int GOTO[][50], char *tokens[], int ntoks){
    int stack[200]; int top=0; stack[top++]=0;
    int ip=0; char *a = tokens[ip];
    printf("\nParsing steps for input:");
    for(int i=0;i<ntoks;i++) printf(" %s", tokens[i]); printf("\n");
    while(1){
        int s = stack[top-1];
        int tid = sym_idx(a);
        if(tid==-1){ printf(" Error: unknown token %s\n", a); return; }
        Action act = ACTION[s][tid];
        if(!act.act[0]){ printf(" Error: no action at state %d on %s\n", s, a); return; }
        if(!strcmp(act.act,"s")){
            printf(" shift to %d on %s\n", act.num, a);
            stack[top++]=tid; stack[top++]=act.num;
            a = tokens[++ip];
        } else if(!strcmp(act.act,"r")){
            int p = act.num;
            printf(" reduce by (%d) %s ->", p, prods[p].lhs);
            for(int i=0;i<prods[p].rhs_len;i++) printf(" %s", prods[p].rhs[i]); printf("\n");
            int pop = prods[p].rhs_len*2; top -= pop;
            int st = stack[top-1];
            int Aidx = sym_idx(prods[p].lhs);
            int goto_state_num = GOTO[st][Aidx];
            if(goto_state_num==-1){ printf(" Error: no goto from state %d on %s\n", st, prods[p].lhs); return; }
            stack[top++]=Aidx; stack[top++]=goto_state_num;
        } else if(!strcmp(act.act,"acc")){
            printf(" accept\n"); return;
        } else if(strchr(act.act,'/')){
            char tmp[32]; strncpy(tmp, act.act, sizeof(tmp)); tmp[sizeof(tmp)-1]='\0';
            char *parts[6]; int pc=0; char *tok = strtok(tmp, "/");
            while(tok && pc<6){ parts[pc++]=tok; tok=strtok(NULL,"/"); }
            int shift_chosen=0;
            for(int i2=0;i2<pc;i2++) if(parts[i2][0]=='s'){ shift_chosen=1; break; }
            if(shift_chosen){
                int tgt = atoi(parts[0]+1); 
                printf(" conflict (s/r) choosing shift to %d on %s (%s)\n", tgt, a, act.act);
                stack[top++]=tid; stack[top++]=tgt; a = tokens[++ip];
            } else {
                
                int prodnum = atoi(parts[0]+1);
                printf(" conflict (r/r) choosing first reduce (%d) %s ->", prodnum, prods[prodnum].lhs);
                for(int k2=0;k2<prods[prodnum].rhs_len;k2++) printf(" %s", prods[prodnum].rhs[k2]); printf("\n");
                int pop = prods[prodnum].rhs_len*2; top -= pop; int st2 = stack[top-1]; int Aidx2 = sym_idx(prods[prodnum].lhs); int goto_state_num2 = GOTO[st2][Aidx2];
                if(goto_state_num2==-1){ printf(" Error: no goto from state %d on %s\n", st2, prods[prodnum].lhs); return; }
                stack[top++]=Aidx2; stack[top++]=goto_state_num2;
            }
        } else {
            printf(" unknown action '%s' in table\n", act.act); return;
        }
    }
}

extern int yylex(void);
extern char *yytext;

char *token_str_for_code(int code){
    if(code==ID) return "id";
    if(code==EQ) return "=";
    if(code==AST) return "*";
    if(code==DOLLAR) return "$";
    return NULL;
}

int main(){
    char *r0[] = {"S"}; add_prod("S'", r0, 1); 
    char *r1[] = {"L","=","R"}; add_prod("S", r1, 3); 
    char *r2[] = {"R"}; add_prod("S", r2, 1); 
    char *r3[] = {"*","R"}; add_prod("L", r3, 2); 
    char *r4[] = {"id"}; add_prod("L", r4, 1); 
    char *r5[] = {"L"}; add_prod("R", r5, 1); 

    build_symbol_list();
    compute_FIRST();
    compute_FOLLOW();
    build_canonical();

    printf("Canonical collection (total states = %d)\n", S_count);
    for(int i=0;i<S_count;i++){ printf("\nState %d:\n", i); print_state(&states[i]); }

    build_tables();
    print_tables();
    report_conflicts();

    char *tokens[200]; int ntoks=0;
    int tok;
    while((tok = yylex())!=0){
        char *ts = token_str_for_code(tok);
        if(ts){
            tokens[ntoks++] = strdup(ts);
            if(ntoks>=199) break;
        }
    }
    if(ntoks==0){
        tokens[ntoks++] = strdup("id");
        tokens[ntoks++] = strdup("=");
        tokens[ntoks++] = strdup("id");
        tokens[ntoks++] = strdup("$");
    } else {
        if(strcmp(tokens[ntoks-1],"$")!=0) tokens[ntoks++] = strdup("$");
    }

    parse_with_table(ACTION_SLR, GOTO_SLR, tokens, ntoks);
    parse_with_table(ACTION_LR0, GOTO_LR0, tokens, ntoks);

    printf("\nDiscussion:\n1) LR(0) constructs tables without lookahead. Conflicts arise because reductions apply on all terminals.\n2) SLR(1) uses FOLLOW sets to restrict reduce actions. Some LR(0) conflicts disappear.\n3) Advantage LR(0): simple. Disadvantage: many conflicts. Advantage SLR: small lookahead, resolves some conflicts. Disadvantage: not as strong as canonical LR(1).\n");

    return 0;
}

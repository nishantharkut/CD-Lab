%option noyywrap
%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct { int s,e; } Feature;

/* process one input line (one row of intensities) */
void process_line(const char *raw, int rowno){
    char line[4096];
    char orig[4096];
    strncpy(line, raw, sizeof(line)-1); line[sizeof(line)-1]=0;
    /* remove trailing newline/cr */
    int L = strlen(line);
    while(L>0 && (line[L-1]=='\n' || line[L-1]=='\r')) line[--L]=0;
    strncpy(orig, line, sizeof(orig)-1); orig[sizeof(orig)-1]=0;

    /* parse intensities */
    int vals[2048], vcnt=0;
    int has_space = 0;
    for(int i=0;i<L;i++) if(isspace((unsigned char)line[i])) { has_space=1; break; }

    if(has_space){
        char buf[4096];
        strncpy(buf, line, sizeof(buf)-1); buf[sizeof(buf)-1]=0;
        char *p = strtok(buf, " \t\r\n");
        while(p){
            vals[vcnt++] = atoi(p);
            p = strtok(NULL, " \t\r\n");
        }
    } else {
        for(int i=0;i<L;i++){
            if(isdigit((unsigned char)line[i])){
                vals[vcnt++] = line[i]-'0';
            }
        }
    }

    if(vcnt < 2){
        printf("Row %d original: %s\n", rowno, orig);
        printf("Row %d d': <none>\n", rowno);
        printf("Row %d d' with maximal features: <none>\n", rowno);
        printf("Row %d longest maximal feature: <none>\n\n", rowno);
        return;
    }

    /* build d' */
    char dprime[4096]; int dlen=0;
    for(int i=0;i<vcnt-1;i++){
        int diff = vals[i+1] - vals[i];
        dprime[dlen++] = (diff>0? '+': (diff<0? '-':'0'));
    }
    dprime[dlen]=0;

    /* find all candidate features matching +{1,} 0{0,3} -{1,} */
    Feature feats[4096]; int fcnt=0;
    for(int i=0;i<dlen;i++){
        if(dprime[i] != '+') continue;
        int p = i;
        while(p < dlen && dprime[p] == '+') p++;
        int plusEnd = p-1;
        for(int z=0; z<=3; z++){
            int zstart = plusEnd+1;
            int zend = zstart + z - 1;
            if(z>0){
                if(zend >= dlen) break;
                int okz = 1;
                for(int k=zstart;k<=zend;k++) if(dprime[k] != '0'){ okz = 0; break; }
                if(!okz) break;
            }
            int mstart = plusEnd + 1 + z;
            if(mstart >= dlen) continue;
            if(dprime[mstart] != '-') continue;
            int m = mstart;
            while(m < dlen && dprime[m] == '-') m++;
            int mend = m-1;
            feats[fcnt].s = i;
            feats[fcnt].e = mend;
            fcnt++;
        }
    }

    /* remove proper-substring features -> keep only maximal features */
    int keep[4096]; for(int i=0;i<fcnt;i++) keep[i]=1;
    for(int i=0;i<fcnt;i++){
        if(!keep[i]) continue;
        for(int j=0;j<fcnt;j++){
            if(i==j || !keep[j]) continue;
            if(feats[i].s >= feats[j].s && feats[i].e <= feats[j].e){
                if(feats[i].s > feats[j].s || feats[i].e < feats[j].e){
                    keep[i] = 0; break;
                }
            }
        }
    }

    /* collect maximal features */
    Feature maxf[4096]; int mcnt=0;
    for(int i=0;i<fcnt;i++) if(keep[i]) maxf[mcnt++] = feats[i];

    if(mcnt==0){
        printf("Row %d original: %s\n", rowno, orig);
        printf("Row %d d': %s\n", rowno, dprime);
        printf("Row %d d' with maximal features: <none>\n", rowno);
        printf("Row %d longest maximal feature: <none>\n\n", rowno);
        return;
    }

    /* sort by start asc, length desc */
    for(int i=0;i<mcnt;i++) for(int j=i+1;j<mcnt;j++){
        int li = maxf[i].e - maxf[i].s + 1;
        int lj = maxf[j].e - maxf[j].s + 1;
        if(maxf[i].s > maxf[j].s || (maxf[i].s==maxf[j].s && li < lj)){
            Feature t = maxf[i]; maxf[i]=maxf[j]; maxf[j]=t;
        }
    }

    /* choose non-overlapping set preferring earlier start and longer length */
    int chosen[4096]; for(int i=0;i<mcnt;i++) chosen[i]=0;
    int last_end = -1;
    for(int i=0;i<mcnt;i++){
        if(maxf[i].s > last_end){
            chosen[i]=1;
            last_end = maxf[i].e;
        }
    }

    /* build output with parentheses */
    char out[8192]; int oi=0;
    int idx = 0;
    for(int i=0;i<mcnt;i++){
        if(!chosen[i]) continue;
        int s = maxf[i].s, e = maxf[i].e;
        for(int t=idx;t<s;t++) out[oi++]=dprime[t];
        out[oi++]='(';
        for(int t=s;t<=e;t++) out[oi++]=dprime[t];
        out[oi++]=')';
        idx = e+1;
    }
    for(int t=idx;t<dlen;t++) out[oi++]=dprime[t];
    out[oi]=0;

    /* find longest chosen maximal feature */
    int best_len=0, bs=-1, be=-1;
    for(int i=0;i<mcnt;i++) if(chosen[i]){
        int Lf = maxf[i].e - maxf[i].s + 1;
        if(Lf > best_len){ best_len = Lf; bs = maxf[i].s; be = maxf[i].e; }
    }

    printf("Row %d original: %s\n", rowno, orig);
    printf("Row %d d': %s\n", rowno, dprime);
    printf("Row %d d' with maximal features: %s\n", rowno, out);
    if(best_len>0){
        char best[256]; int bi=0;
        for(int t=bs;t<=be;t++) best[bi++]=dprime[t];
        best[bi]=0;
        printf("Row %d longest maximal feature: %s (length %d)\n\n", rowno, best, best_len);
    } else {
        printf("Row %d longest maximal feature: <none>\n\n", rowno);
    }
}
%}

%%

.*\n    { static int row_counter = 0; process_line(yytext, ++row_counter); }
<<EOF>> { /* end */ }

%%

int main(int argc, char **argv){
    yylex();
    return 0;
}

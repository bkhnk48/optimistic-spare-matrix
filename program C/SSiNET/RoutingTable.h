#include "RoutingTable.c"

int setHavingSuffix(int **HavingSuffix, int numOfPorts);

int setHavingPrefix(int **HavingPrefix, int numOfPorts);

int setHavingCorePrefix(int **HavingCorePrefix, int numOfPorts);

void setSuffix(int **Suffix, int numOfPorts);
void setPrefix(int **Prefix, int numOfPorts);
void setCorePrefix(int **CorePrefix, int numOfPorts);

void showTwoLevelsRoutingTable(int **HavingSuffix, int **HavingPrefix, int **HavingCorePrefix
                        , int **Suffix, int **Prefix, int **CorePrefix, int numOfPorts
                            );
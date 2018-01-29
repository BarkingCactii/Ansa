int main ()
{
    Smt_InitHelp ( );
    InitUserColours ( );

    pclrchar ( BKGROUND_CHAR );
    pclrattr ( REVNORML );

    Sdb_AccessDatabase ();
}



-- David R. Musser and Alexander A. Stepanov:
-- Ada Generic Library: Linear List Processing Packages, 1989

/* {{{ *//* }}} */

  package Double_Ended_Lists is
 
    type Del is limited private;
	
  private
 
    type Del is record
      First   : Sequence := Nil;
      Current : Sequence := Nil;
      Last    : Sequence := Nil;
    end record;
 
  end Double_Ended_Lists; 

	
  
  with Singly_Linked_Lists;
  package body Double_Ended_Lists is
 
    package Regular_Lists is 
        new Singly_Linked_Lists(Element, Sequence, Nil, First,
                 Next, Construct, Set_First, Set_Next, Free_Construct);

    procedure Concatenate(S1, S2 : in out Del) is 
    begin
      if Is_Empty(S1) then
        S1 := S2;
        Make_Empty(S2);
      elsif not Is_Empty(S2) then
        Set_Next(S1.Last, S2.First);
        S1.Last := S2.Last;
        Make_Empty(S2); 
      end if;
    end Concatenate;

    procedure Drop_Head(S : in out Del) is 
      Next_One : Sequence;
    begin
      if Is_End(S) then
        Regular_Lists.Free_Sequence(S.First);
        Make_Empty(S);
      else
        Next_One := Next(S.Current);
        if Regular_Lists.Is_End(Next_One) then
          Regular_Lists.Free_Sequence(S.First);
          Make_Empty(S);
        else
          Set_Next(S.Current, Nil);
          Regular_Lists.Free_Sequence(S.First);
          S.First := Next_One;
          Initialize(S);
        end if;
      end if;
    end Drop_Head;

    procedure Drop_Tail(S : in out Del) is 
      Next_One : Sequence;
    begin
      if not Is_End(S) then
        Next_One := Next(S.Current);
        if not Regular_Lists.Is_End(Next_One) then
          Set_Next(S.Current, Nil);
          Regular_Lists.Free_Sequence(Next_One);
          S.Last := S.Current;
        end if;
      end if;
    end Drop_Tail;

    procedure Split(S1, S2 : in out Del) is 
      Next_One : Sequence;
    begin
      Free(S2);
      if not Is_End(S1) then
        Next_One := Next(S1.Current);
        if not Regular_Lists.Is_End(Next_One) then
          Set_Next(S1.Current, Nil);
          S2.First := Next_One;
          S2.Current := Next_One;
          S2.Last := S1.Last;
          S1.Last := S1.Current;
        end if;
      end if;
    end Split;

  end Double_Ended_Lists;

/* {{{ *//* }}} */

function Delete_Duplicates(S : Cell)
	   return Cell is 
  Tail, To_Be_Done, I : Cell := S;
begin
  if not Is_End(To_Be_Done) then
	Advance(To_Be_Done);
	while not Is_End(To_Be_Done) loop
	  I := S;
	  while I /= To_Be_Done and then not Test(I, To_Be_Done) loop
		Advance(I);
	  end loop;
	  if I = To_Be_Done then
		Tail := To_Be_Done;
		Advance(To_Be_Done);
	  else
		I := To_Be_Done;
		Advance(To_Be_Done);
		Set_Next(Tail, To_Be_Done);
		Free(I);
	  end if;
	end loop;
  end if;
  return S;
end Delete_Duplicates;

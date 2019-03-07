(
s.waitForBoot{
	{
		var w, x, y, p, a, b, c, d;

		p = PinkNoise.ar; // source

		// B-format encode
		#w, x, y = PanB2.ar(p, MouseX.kr(-1,1), 0.1);

		// B-format decode to quad
		// #a, b, c, d = DecodeB2.ar(4, w, x, y);
		#a, b, c, d = SIMDTest.ar(4, w, x, y);

		[a, b, d, c] // reorder to my speaker arrangement: Lf Rf Lr Rr
	}.play(outbus: 2);
	s.scope(4,2);
}
)

/*
SIMDTest : MultiOutUGen {
// DecodeB2 : MultiOutUGen {
	*ar { arg numChans, w, x, y, orientation = 0.5;
		^this.multiNew('audio', numChans, w, x, y, orientation = 0.5 )
	}
	*kr { arg numChans, w, x, y, orientation = 0.5;
		^this.multiNew('control', numChans, w, x, y, orientation = 0.5 )
	}
	init { arg numChans ... theInputs;
		inputs = theInputs;
		channels = Array.fill(numChans, { arg i; OutputProxy(rate,this, i) });
		^channels
	}
	checkInputs { ^this.checkNInputs(3) }
}
*/
function v = SWIG_IND1()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 16);
  end
  v = vInitialized;
end

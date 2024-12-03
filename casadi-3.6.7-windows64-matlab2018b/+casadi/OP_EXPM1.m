function v = OP_EXPM1()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 111);
  end
  v = vInitialized;
end

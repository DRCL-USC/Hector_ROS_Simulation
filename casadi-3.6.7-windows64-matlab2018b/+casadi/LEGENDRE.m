function v = LEGENDRE()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 124);
  end
  v = vInitialized;
end

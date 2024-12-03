function v = OP_FABS()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 46);
  end
  v = vInitialized;
end
